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

#include "Tokenizer.h"

namespace ExLibris
{


#define TYPE_CLASS_NAME(_name) CharacterType##_name
#define TYPE_CLASS(_name, _condition) \
	class TYPE_CLASS_NAME(_name) { \
		public: \
			static inline bool IsKnown(int c) { \
				return _condition; \
			} \
	}

	TYPE_CLASS(Digit, (c >= '0' && c <= '9'));
	TYPE_CLASS(Octal, (c >= '0' && c <= '7'));
	TYPE_CLASS(Hexadecimal, (
		(c >= '0' && c <= '9') || 
		(c >= 'a' && c <= 'f') ||
		(c >= 'A' && c <= 'F')
	));
	TYPE_CLASS(Alphabetical, (
		(c >= 'A' && c <= 'Z') ||
		(c >= 'a' && c <= 'z')
	));
	TYPE_CLASS(Identifier, (
		(c >= 'A' && c <= 'Z') ||
		(c >= 'a' && c <= 'z') ||
		(c >= '0' && c <= '9') ||
		c == '_'
	));
	TYPE_CLASS(Symbol, (
		c == '!' || c == '\"' || c == '#' || c == '$' || c == '%' ||
		c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' ||
		c == '+' || c == ',' || c == '-' || c == '.' || c == '/' ||
		c == ':' || c == ';' || c == '<' || c == '=' || c == '>' ||
		c == '?' || c == '@' || c == '[' || c == '\\' || c == ']' ||
		c == '^' || c == '_' || c == '`' || c == '{' || c == '|' ||
		c == '}' || c == '~'
	));
	TYPE_CLASS(Whitespace, (c == ' ' || c == '\t'));
	TYPE_CLASS(NewLine, (c == '\r' || c == '\n'));

	Tokenizer::Tokenizer(std::basic_istream<char>* a_Stream)
		: m_Options(eOption_Identifiers)
		, m_Stream(nullptr)
		, m_StreamEnd(false)
		, m_TabWidth(4)
		, m_RevertToken(false)
		, m_CharactersConsumedCount(0)
		, m_NumberFoundDot(false)
		, m_CharacterCurrent(-1)
		, m_Column(0)
		, m_Line(0)
	{
		SetInput(a_Stream);
	}
	
	Tokenizer::~Tokenizer()
	{
	}

	bool Tokenizer::IsOptionEnabled(Option a_Option) const
	{
		return ((m_Options & a_Option) != 0);
	}

	void Tokenizer::EnableOptions(unsigned int a_Options)
	{
		m_Options |= a_Options;
	}

	bool Tokenizer::IsOptionDisabled(Option a_Option) const
	{
		return ((m_Options & a_Option) == 0);
	}

	void Tokenizer::DisableOptions(unsigned int a_Options)
	{
		m_Options = (m_Options & ~a_Options);
	}

	void Tokenizer::SetInput(std::basic_istream<char>* a_Stream)
	{
		m_Stream = a_Stream;
		m_CharacterQueue.clear();

		m_Column = 1;
		m_Line = 1;

		m_TokenCurrent.type = Token::eType_End;
		m_TokenCurrent.text.clear();
		m_TokenCurrent.column = m_Column;
		m_TokenCurrent.line = m_Line;
	}

	void Tokenizer::SetTabWidth(size_t a_Width)
	{
		m_TabWidth = std::max<size_t>(1, a_Width);
	}

	const Token& Tokenizer::GetCurrentToken() const
	{
		return m_TokenCurrent;
	}

	bool Tokenizer::IsNextTokenAvailable() const
	{
		return ((m_Stream != nullptr && !m_Stream->eof()) || (m_CharacterQueue.size() > 0));
	}

	bool Tokenizer::ReadToken()
	{
		if (m_RevertToken)
		{
			m_RevertToken = false;

			return (m_TokenCurrent.type != Token::eType_End);
		}

		m_CharactersRead.clear();
		m_CharactersConsumedCount = 0;

		m_NumberFoundDot = false;
		m_StringDelimiter = -1;

		m_TokenCurrent.text.clear();
		m_TokenCurrent.column = m_Column;
		m_TokenCurrent.line = m_Line;

		if (!_NextCharacter())
		{
			m_TokenCurrent.type = Token::eType_End;

			return false;
		}

		if (_Match('.'))
		{
			m_TokenCurrent.type = Token::eType_Number;
		}
		else if (_Match('0'))
		{
			m_TokenCurrent.type = Token::eType_Octal;
		}
		else if (_Match('\r'))
		{
			m_TokenCurrent.type = Token::eType_Unprintable;
		}
		else if (_Match('\n'))
		{
			m_TokenCurrent.type = Token::eType_NewLine;
		}
		else if (_MatchEither('\"', '\''))
		{
			m_TokenCurrent.type = Token::eType_String;
		}
		else if (_MatchEither(' ', '\t'))
		{
			m_TokenCurrent.type = Token::eType_Whitespace;
		}
		else if (_MatchType<CharacterTypeDigit>())
		{
			m_TokenCurrent.type = Token::eType_Integer;
		}
		else if (IsOptionEnabled(Tokenizer::eOption_Identifiers) && _MatchType<CharacterTypeIdentifier>())
		{
			m_TokenCurrent.type = Token::eType_Identifier;
		}
		else if (_ConsumeType<CharacterTypeSymbol>())
		{
			// note that a symbol is always a single character,
			// so we don't need to call the recursive method

			m_TokenCurrent.type = Token::eType_Symbol;

			return true;
		}
		else
		{
			m_TokenCurrent.type = Token::eType_Text;
		}

		return _RecursiveReadToken();
	}

	void Tokenizer::RevertToken()
	{
		m_RevertToken = true;
	}

	bool Tokenizer::_RecursiveReadToken()
	{
		bool handled = false;

		switch (m_TokenCurrent.type)
		{

		case Token::eType_Identifier:
			{
				if (m_CharactersConsumedCount == 0 && _Match('_'))
				{
					if (_NextCharacter())
					{
						_AddToToken('_');

						while (_ConsumeType<CharacterTypeIdentifier>())
						{
							if (!_NextCharacter())
							{
								return true;
							}
						}

						if (m_CharactersConsumedCount == 1)
						{
							m_TokenCurrent.type = Token::eType_Symbol;
						}

						_Revert(1);

						return true;
					}
					else
					{
						m_TokenCurrent.type = Token::eType_Symbol;
						
						_AddCurrentToToken();

						handled = true;
					}
				}
				else
				{
					while (_ConsumeType<CharacterTypeIdentifier>())
					{
						if (!_NextCharacter())
						{
							return true;
						}
					}

					_Revert(1);

					return true;
				}

			} break;

		case Token::eType_Text:
			{
				while (_ConsumeType<CharacterTypeAlphabetical>())
				{
					if (!_NextCharacter())
					{
						return true;
					}
				}

				_Revert(1);

				return true;

			} break;

		case Token::eType_String:
			{
				if (_Consume(m_StringDelimiter))
				{
					return true;
				}
				else if (m_CharactersConsumedCount == 0 && _ConsumeEither('\'', '\"'))
				{
					m_StringDelimiter = m_CharacterCurrent;
				}
				else if (_Match('\r'))
				{
					if (_NextCharacter())
					{
						if (_Match('\n'))
						{
							// undo consumed

							_Revert(m_CharactersRead.size());

							// quote character was just a symbol

							m_TokenCurrent.type = Token::eType_Symbol;
							m_TokenCurrent.text.clear();
							_NextCharacter();
							_AddCurrentToToken();

							return true;
						}
						else
						{
							// just a line feed

							_Revert(1);

							_AddToToken('\r');
						}
					}
					else
					{
						 // just a line feed

						_AddCurrentToToken();
					}
				}
				else if (_Match('\n') || !_IsNextCharacterAvailable())
				{
					// undo consumed

					_Revert(m_CharactersRead.size());

					// quote character was just a symbol

					m_TokenCurrent.type = Token::eType_Symbol;
					m_TokenCurrent.text.clear();
					_NextCharacter();
					_AddCurrentToToken();

					return true;
				}
				else
				{
					_AddCurrentToToken();
				}

			} break;

		case Token::eType_Integer:
			{
				if (_Match('.'))
				{
					m_TokenCurrent.type = Token::eType_Number;

					return _RecursiveReadToken();
				}
				else if (_MatchEither('e', 'E'))
				{
					m_TokenCurrent.type = Token::eType_Scientific;

					m_ScientificTypeRestore = Token::eType_Integer;

					return _RecursiveReadToken();
				}
				else if (!_ConsumeType<CharacterTypeDigit>())
				{
					handled = true;
				}

			} break;

		case Token::eType_Octal:
			{
				if (_Match('.'))
				{
					m_TokenCurrent.type = Token::eType_Number;

					return _RecursiveReadToken();
				}
				else if (_MatchEither('e', 'E'))
				{
					m_TokenCurrent.type = Token::eType_Scientific;

					if (m_CharactersConsumedCount == 1)
					{
						// just a zero

						m_ScientificTypeRestore = Token::eType_Integer;
					}
					else
					{
						m_ScientificTypeRestore = Token::eType_Octal;
					}

					return _RecursiveReadToken();
				}
				else if (m_CharactersConsumedCount == 1 && _Consume('x'))
				{
					m_TokenCurrent.type = Token::eType_Hexadecimal;
				}
				else if (!_ConsumeType<CharacterTypeOctal>())
				{
					if (m_CharactersConsumedCount == 1)
					{
						// just a zero

						m_TokenCurrent.type = Token::eType_Integer;
					}

					handled = true;
				}

			} break;

		case Token::eType_Hexadecimal:
			{
				if (!_ConsumeType<CharacterTypeHexadecimal>())
				{
					if (m_CharactersConsumedCount == 2)
					{
						// was a zero with text

						m_TokenCurrent.type = Token::eType_Integer;

						_Revert(3);

						m_TokenCurrent.text.clear();

						_NextCharacter();
						_AddCurrentToToken();

						return true;
					}
					else
					{
						handled = true;
					}
				}

			} break;

		case Token::eType_Number:
			{
				if (_Consume('f') || _Consume('F'))
				{
					// no more digits after specifier

					return true;
				}
				else if (_MatchEither('e', 'E'))
				{
					m_TokenCurrent.type = Token::eType_Scientific;

					m_ScientificTypeRestore = Token::eType_Number;

					return _RecursiveReadToken();
				}
				else if (m_CharacterCurrent == '.')
				{
					if (m_NumberFoundDot)
					{
						if (m_CharactersConsumedCount == 0)
						{
							// just a dot

							m_TokenCurrent.type = Token::eType_Symbol;
						}

						handled = true;
					}
					else
					{
						if (_NextCharacter())
						{
							if (_MatchEither('f', 'F'))
							{
								_AddToToken('.');
								_AddCurrentToToken();

								return true;
							}
							else if (_MatchType<CharacterTypeDigit>())
							{
								_AddToToken('.');
								_AddCurrentToToken();

								m_NumberFoundDot = true;
							}
							else
							{
								if (m_CharactersConsumedCount == 0)
								{
									// leading dot

									_Revert(1);

									m_TokenCurrent.type = Token::eType_Symbol;
									
									_AddToToken('.');
								}
								else
								{
									// trailing dot

									_Revert(2);

									m_TokenCurrent.type = Token::eType_Integer;
								}

								return true;
							}
						}
						else
						{
							if (m_CharactersConsumedCount == 0)
							{
								// dot only

								m_TokenCurrent.type = Token::eType_Symbol;
								
								_AddToToken('.');
							}
							else
							{
								// trailing dot

								_Revert(1);

								m_TokenCurrent.type = Token::eType_Integer;
							}

							return true;
						}
					}
				}
				else if (!_ConsumeType<CharacterTypeDigit>())
				{
					handled = true;
				}

			} break;

		case Token::eType_Scientific:
			{
				if (_MatchEither('e', 'E'))
				{
					int queued[3] = { m_CharacterCurrent, 0, 0 };
					int found = 1;

					size_t found_start = m_CharactersRead.size();

					if (_NextCharacter() && _MatchEither('-', '+'))
					{
						queued[found++] = m_CharacterCurrent;

						if (_NextCharacter() && _MatchType<CharacterTypeDigit>())
						{
							queued[found++] = m_CharacterCurrent;
						}
					}

					if (found == 3)
					{
						_AddToToken(queued[0]);
						_AddToToken(queued[1]);
						_AddToToken(queued[2]);
					}
					else
					{
						_Revert(m_CharactersRead.size() - found_start + 1);

						m_TokenCurrent.type = m_ScientificTypeRestore;

						return true;
					}
				}
				else if (!_IsNextCharacterAvailable() || _ConsumeEither('f', 'F') || !_ConsumeType<CharacterTypeDigit>())
				{
					handled = true;
				}

			} break;

		case Token::eType_Whitespace:
			{
				if (_Consume('\t'))
				{
					// tab position is determined from index, not column

					m_Column = m_TokenCurrent.column + (m_TabWidth - ((m_TokenCurrent.column - 1) % m_TabWidth));

					return true;
				}
				else if (_Consume(' '))
				{
					return true;
				}
				else
				{
					handled = true;
				}

			} break;

		case Token::eType_Unprintable:
			{
				if (_Consume('\r'))
				{
					if (_NextCharacter() && _Match('\n'))
					{
						// carriage return and newline combine to a newline

						m_TokenCurrent.type = Token::eType_NewLine;

						return _RecursiveReadToken();
					}
					else
					{
						// just a carriage return

						handled = true;
					}
				}

			} break;

		case Token::eType_NewLine:
			{
				if (_Consume('\n'))
				{
					m_Column = 1;
					m_Line++;

					return true;
				}

			} break;

		default:
			{
				return false;

			} break;

		}

		bool next_available = _IsNextCharacterAvailable();

		if (handled)
		{
			if (next_available)
			{
				_Revert(1);
			}

			return true;
		}
		else
		{
			if (next_available)
			{
				_NextCharacter();
			}

			return _RecursiveReadToken();
		}
	}

	bool Tokenizer::_IsNextCharacterAvailable() const
	{
		return ((m_Stream != nullptr) && !m_Stream->eof()) || (m_CharacterQueue.size() > 0);
	}

	bool Tokenizer::_NextCharacter()
	{
		bool next_available = false;

		if (m_CharacterQueue.size() > 0)
		{
			m_CharacterCurrent = m_CharacterQueue.front();
			m_CharacterQueue.pop_front();

			next_available = true;
		}
		else if (m_Stream != nullptr && !m_Stream->eof())
		{
			m_CharacterCurrent = m_Stream->get();

			next_available = !m_Stream->eof();
		}

		if (next_available)
		{
			m_CharactersRead.push_back(m_CharacterCurrent);

			m_Column++;
		}

		return next_available;
	}

	void Tokenizer::_Revert(int a_Count)
	{
		int added = 0;

		while (added < a_Count && m_CharactersRead.size() > 0)
		{
			m_CharacterQueue.push_front(m_CharactersRead.back());
			m_CharactersRead.pop_back();

			++added;
		}

		m_Column -= added;
	}

}; // namespace ExLibris