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

	Tokenizer::Tokenizer(std::basic_istream<char>* a_Stream)
		: m_Stream(a_Stream)
	{
	}
	
	Tokenizer::~Tokenizer()
	{
	}

	void Tokenizer::SetInput(std::basic_istream<char>* a_Stream)
	{
		m_Stream = a_Stream;
	}

	const Token& Tokenizer::GetCurrentToken() const
	{
		return m_TokenCurrent;
	}

	bool Tokenizer::IsNextTokenAvailable() const
	{
		return (m_Stream != nullptr && !m_Stream->eof());
	}

	bool Tokenizer::ReadToken()
	{
		if (!IsNextTokenAvailable())
		{
			return false;
		}

		m_TokenCurrent.text.clear();

		while (1)
		{
			int byte = m_Stream->get();
			if (byte < 0)
			{
				break;
			}

			m_TokenCurrent.text.push_back((char)byte);
		}

		m_TokenCurrent.type = Token::eType_String;
		m_TokenCurrent.column = 0;
		m_TokenCurrent.line = 0;

		return true;
	}

}; // namespace ExLibris"