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
#define TYPE_CLASS(_name, ...) \
	class TYPE_CLASS_NAME(_name) { \
		public: \
			static inline bool IsKnown(int a_Character) { \
				static const int identifiers[] = { __VA_ARGS__ }; \
				static const int identifiers_size = sizeof(identifiers) / sizeof(int); \
				for (int i = 0; i < identifiers_size; ++i) { \
					if (a_Character == identifiers[i]) { return true; } \
				} \
				return false; \
			} \
	} \

	TYPE_CLASS(Digit,
		'0', '1', '2', '3', '4',
		'5', '6', '7', '8', '9'
	);
	TYPE_CLASS(Symbol, 
		'!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', 
		',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', 
		'[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'
	);
	TYPE_CLASS(Whitespace,
		' ', '\t'
	);
	TYPE_CLASS(NewLine,
		'\r', '\n'
	);

	Tokenizer::Tokenizer(std::basic_istream<char>* a_Stream)
		: m_Stream(nullptr)
		, m_CharacterCurrent(-1)
		, m_Column(0)
		, m_Line(0)
	{
		SetInput(a_Stream);
	}
	
	Tokenizer::~Tokenizer()
	{
	}

	void Tokenizer::SetInput(std::basic_istream<char>* a_Stream)
	{
		m_Stream = a_Stream;

		m_Column = 1;
		m_Line = 1;

		if (m_Stream != nullptr)
		{
			m_CharacterCurrent = m_Stream->get();
		}
		else
		{
			m_CharacterCurrent = -1;
		}
	}

	const Token& Tokenizer::GetCurrentToken() const
	{
		return m_TokenCurrent;
	}

	bool Tokenizer::IsNextTokenAvailable() const
	{
		return (m_Stream != nullptr && !m_Stream->eof() && m_CharacterCurrent != -1);
	}

	bool Tokenizer::ReadToken()
	{
		m_TokenCurrent.text.clear();

		m_TokenCurrent.column = m_Column;
		m_TokenCurrent.line = m_Line;

		if (!IsNextTokenAvailable())
		{
			m_TokenCurrent.type = Token::eType_End;

			return false;
		}

		m_TokenCurrent.type = _GetTypeForCharacter(m_CharacterCurrent);

		m_TokenCurrent.text.push_back((char)m_CharacterCurrent);

		bool result = false;

		switch (m_TokenCurrent.type)
		{

		case Token::eType_Integer:
			{
				result = _ReadOneOrMore();

			} break;

		case Token::eType_Symbol:
			{
				// check if it's a negative number

				if (m_CharacterCurrent == '-')
				{
					if (_TryReadOneOrMore<CharacterTypeDigit>())
					{
						m_TokenCurrent.type = Token::eType_Integer;

						result = true;
					}
					else
					{
						result = _ReadOne();
					}
				}
				else
				{
					result = _ReadOne();
				}

			} break;

		case Token::eType_Whitespace:
			{
				result = _ReadOneOrMore();

			} break;

		case Token::eType_NewLine:
			{
				result = _ReadOneOrMore();

				m_Column = 1;
				m_Line++;

			} break;

		case Token::eType_Text:
			{
				result = _ReadOneOrMore();

			} break;

		}

		return result;
	}

	bool Tokenizer::_TryReadCharacter()
	{
		if (m_CharacterQueue.size() > 0)
		{
			m_CharacterCurrent = m_CharacterQueue.front();
			m_CharacterQueue.pop();
		}
		else
		{
			m_CharacterCurrent = m_Stream->get();
		}

		m_Column++;

		return (m_CharacterCurrent != -1);
	}

	void Tokenizer::_QueueCurrentCharacter()
	{
		m_CharacterQueue.push(m_CharacterCurrent);
		m_Column--;
	}

	bool Tokenizer::_ReadOne()
	{
		_TryReadCharacter();

		return true;
	}

	bool Tokenizer::_ReadOneOrMore()
	{
		while (1)
		{
			if (!_TryReadCharacter())
			{
				break;
			}

			Token::Type token_type_found = _GetTypeForCharacter(m_CharacterCurrent);
			if (token_type_found != m_TokenCurrent.type)
			{
				break;
			}

			m_TokenCurrent.text.push_back((char)m_CharacterCurrent);
		}

		return true;
	}

	Token::Type Tokenizer::_GetTypeForCharacter(int a_Character)
	{
		Token::Type found_type = Token::eType_Text;

		if (_IsCharacterOfType<CharacterTypeDigit>(a_Character))
		{
			found_type = Token::eType_Integer;
		}
		else if (_IsCharacterOfType<CharacterTypeSymbol>(a_Character))
		{
			found_type = Token::eType_Symbol;
		}
		else if (_IsCharacterOfType<CharacterTypeWhitespace>(a_Character))
		{
			found_type = Token::eType_Whitespace;
		}
		else if (_IsCharacterOfType<CharacterTypeNewLine>(a_Character))
		{
			found_type = Token::eType_NewLine;
		}

		return found_type;
	}

}; // namespace ExLibris"