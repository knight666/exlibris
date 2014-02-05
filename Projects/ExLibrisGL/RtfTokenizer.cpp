/*
 * This file is a part of the ExLibris project.
 *
 * Copyright (C) 2013 Quinten Lansu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

#include "ExLibrisGL.PCH.h"

#include "RtfTokenizer.h"

namespace ExLibris {
namespace Rtf {

#define TYPE_CLASS_NAME(_name) CharacterType##_name
#define TYPE_CLASS(_name, _condition) \
	class TYPE_CLASS_NAME(_name) { \
		public: \
			static inline bool Match(char c) { \
				return _condition; \
			} \
	}

	TYPE_CLASS(Digit, (c >= '0' && c <= '9'));
	TYPE_CLASS(Alphabetical, (
		(c >= 'A' && c <= 'Z') ||
		(c >= 'a' && c <= 'z')
	));
	TYPE_CLASS(Escaped, (c == '{' || c == '}' || c == '\\'));

	Tokenizer::Tokenizer()
		: m_Input(nullptr)
		, m_Column(1)
		, m_Line(1)
		, m_Group(0)
		, m_Character(0)
		, m_CharactersQueued(0)
		, m_Consumed(0)
	{
		m_Current.type = RtfToken::eParseType_End;
		m_Current.value.clear();
		m_Current.parameter = -1;
		m_Current.group = 0;
		m_Current.column = 1;
		m_Current.line = 1;
	}

	Tokenizer::~Tokenizer()
	{
	}

	void Tokenizer::SetInput(std::basic_istream<char>* a_Stream)
	{
		m_Input = a_Stream;

		m_Column = 1;
		m_Line = 1;
		m_Group = 0;

		m_Current.type = RtfToken::eParseType_End;
		m_Current.value.clear();
		m_Current.parameter = -1;
		m_Current.group = 0;
		m_Current.column = 1;
		m_Current.line = 1;
	}

	bool Tokenizer::IsNextAvailable() const
	{
		return (m_Input != nullptr && !m_Input->eof());
	}

	const RtfToken& Tokenizer::GetCurrent() const
	{
		return m_Current;
	}

	bool Tokenizer::Read()
	{
		m_Current.value.clear();
		m_Current.parameter = -1;
		m_Current.group = m_Group;
		m_Current.column = m_Column;
		m_Current.line = m_Line;

		m_CharactersRead.clear();
		m_Consumed = 0;

		if (!_NextCharacter())
		{
			m_Current.type = RtfToken::eParseType_End;

			return false;
		}

		if (_Match('{'))
		{
			m_Current.type = RtfToken::eParseType_GroupOpen;
			m_Current.group = ++m_Group;

			return true;
		}
		else if (_Match('}'))
		{
			m_Current.type = RtfToken::eParseType_GroupClose;
			m_Current.group = --m_Group;

			return true;
		}
		else if (_Match('\\'))
		{
			m_Current.type = RtfToken::eParseType_Command;
		}
		else if (_Match(';'))
		{
			m_Current.type = RtfToken::eParseType_Value;
		}
		else if (_ParseNewLine())
		{
			return Read();
		}
		else
		{
			_AddCurrentToToken();

			m_Current.type = RtfToken::eParseType_Text;
		}

		return _RecursiveRead();
	}

	bool Tokenizer::SkipCurrentGroup()
	{
		if (m_Group <= 0)
		{
			return false;
		}

		m_CharactersRead.clear();
		m_Consumed = 0;

		int group_target = m_Group - 1;

		while (_NextCharacter())
		{
			if (_Match('{'))
			{
				m_Group++;
			}
			else if (_Match('}'))
			{
				m_Group--;

				if (m_Group == group_target)
				{
					break;
				}
			}
		}

		return ((m_CharactersRead.size() > 0) && (m_Group == group_target));
	}

	bool Tokenizer::_NextCharacter()
	{
		bool next = false;

		if (m_CharactersQueued.size() > 0)
		{
			m_Character = m_CharactersQueued.front();
			m_CharactersQueued.pop_front();

			next = true;
		}
		else if (m_Input != nullptr && !m_Input->eof())
		{
			m_Character = (char)m_Input->get();

			next = !m_Input->eof();
		}

		if (next)
		{
			m_CharactersRead.push_back(m_Character);

			m_Column++;
		}

		return next;
	}

	bool Tokenizer::_ParseNewLine()
	{
		switch (m_Character)
		{

		case '\n':
			{
				m_CharactersRead.pop_back();

				m_Column = 1;
				m_Line++;

				return true;

			} break;

		case '\r':
			{
				m_CharactersRead.pop_back();

				if (_NextCharacter() && _Match('\n'))
				{
					m_CharactersRead.pop_back();

					m_Column = 1;
					m_Line++;
				}
				
				return true;

			} break;

		default:
			{
				return false;

			} break;

		}
	}

	void Tokenizer::_Revert(int a_Count)
	{
		int added = 0;

		while (added < a_Count && m_CharactersRead.size() > 0)
		{
			m_CharactersQueued.push_front(m_CharactersRead.back());
			m_CharactersRead.pop_back();

			++added;
		}

		m_Column -= added;
	}

	bool Tokenizer::_RecursiveRead()
	{
		switch (m_Current.type)
		{

		case RtfToken::eParseType_CommandExtended:
		case RtfToken::eParseType_Command:
			{
				if (!_Match('\\'))
				{
					m_Current.type = RtfToken::eParseType_Invalid;

					return true;
				}
				
				if (!_NextCharacter() || _Match('\\'))
				{
					// it's escaped, skip the second slash

					m_Current.type = RtfToken::eParseType_Text;
					_AddToToken('\\');

					return _RecursiveRead();
				}

				if (m_Current.type != RtfToken::eParseType_CommandExtended && _Match('*'))
				{
					if (!_NextCharacter() || _ParseNewLine())
					{
						// not enough input or new line

						m_Current.type = RtfToken::eParseType_Invalid;
						m_Current.value = m_CharactersRead;

						return true;
					}

					// extended control word

					m_Current.type = RtfToken::eParseType_CommandExtended;

					return _RecursiveRead();
				}

				// read command

				int command_start = m_Consumed;

				if (_ConsumeType<CharacterTypeAlphabetical>())
				{
					while (_ReadTyped<CharacterTypeAlphabetical>())
					{
						_AddCurrentToToken();
					}
				}

				if ((m_Consumed - command_start) == 0)
				{
					// just a forwards slash

					m_Current.type = RtfToken::eParseType_Invalid;
					m_Current.value = m_CharactersRead;

					return true;
				}

				if (!_NextCharacter() || _ParseNewLine())
				{
					// end of input

					return true;
				}

				if (_Match('-') || _MatchType<CharacterTypeDigit>())
				{
					// read parameter

					std::string parameter;

					if (_Match('-'))
					{
						// negative parameter

						parameter.push_back(m_Character);
						m_Consumed++;

						if (!_NextCharacter() || _ParseNewLine())
						{
							m_Current.type = RtfToken::eParseType_Invalid;
							m_Current.value = m_CharactersRead;

							return true;
						}
					}

					if (_MatchType<CharacterTypeDigit>())
					{
						parameter.push_back(m_Character);
						m_Consumed++;

						while (_ReadTyped<CharacterTypeDigit>())
						{
							parameter.push_back(m_Character);
							m_Consumed++;
						}

						m_Current.parameter = atoi(parameter.c_str());
					}

					if (_NextCharacter() && !_ParseNewLine() && !_Match(' '))
					{
						// wasn't part of the command

						_Revert(1);
					}
				}
				else if (_Match(' '))
				{
					// consume trailing space, but don't add it to command

					return true;
				}
				else
				{
					// wasn't part of the command

					_Revert(1);
				}

			} break;

		case RtfToken::eParseType_Text:
			{
				while (_NextCharacter())
				{
					if (_Match('\\'))
					{
						// check if it's an escaped character

						if (!_NextCharacter())
						{
							// slash at end of input

							_AddToToken('\\');

							return true;
						}

						if (!_MatchType<CharacterTypeEscaped>())
						{
							// not an escaped character

							_Revert(2);

							return true;
						}

						_AddCurrentToToken();
					}
					else if (_Match('{') || _Match('}'))
					{
						// group control word

						_Revert(1);

						return true;
					}
					else if (_Match(';'))
					{
						// consume control word, but don't add it to the value

						m_Current.type = RtfToken::eParseType_Value;

						return true;
					}
					else if (!_ParseNewLine())
					{
						_AddCurrentToToken();
					}
				}

			} break;

		}

		return true;
	}

}; // namespace Rtf
}; // namespace ExLibris