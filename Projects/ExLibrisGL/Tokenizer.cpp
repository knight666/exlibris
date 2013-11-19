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

	class CharacterTypeWhitespace
	{

	public:

		static inline bool IsKnown(int a_Character)
		{
			static const int identifiers[] = { ' ', '\t' };
			static const int identifiers_size = sizeof(identifiers) / sizeof(int);

			for (int i = 0; i < identifiers_size; ++i)
			{
				if (a_Character == identifiers[i])
				{
					return true;
				}
			}
			
			return false;
		}

	};

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
		if (!IsNextTokenAvailable())
		{
			return false;
		}

		// determine token type

		m_TokenCurrent.type = _GetTypeForCharacter(m_CharacterCurrent);

		m_TokenCurrent.text.clear();
		m_TokenCurrent.text.push_back((char)m_CharacterCurrent);

		m_TokenCurrent.column = m_Column;
		m_TokenCurrent.line = m_Line;

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

	bool Tokenizer::_TryReadCharacter()
	{
		m_CharacterCurrent = m_Stream->get();
		bool result = (m_CharacterCurrent != -1);

		if (result)
		{
			m_Column++;
		}

		return result;
	}

	Token::Type Tokenizer::_GetTypeForCharacter(int a_Character)
	{
		Token::Type found_type = Token::eType_String;

		if (_IsCharacterOfType<CharacterTypeWhitespace>(a_Character))
		{
			found_type = Token::eType_Whitespace;
		}

		return found_type;
	}

}; // namespace ExLibris"