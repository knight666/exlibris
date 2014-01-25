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

	Tokenizer::Tokenizer()
		: m_Input(nullptr)
		, m_Column(1)
		, m_Line(1)
		, m_Group(0)
		, m_Character(0)
	{
		m_Current.type = RtfToken::eParseType_End;
		m_Current.value.clear();
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
		m_Current.column = m_Column;
		m_Current.line = m_Line;
		m_Current.group = m_Group;

		if (!_NextCharacter())
		{
			m_Current.type = RtfToken::eParseType_End;

			return false;
		}

		if (_Match('{'))
		{
			m_Current.type = RtfToken::eParseType_GroupOpen;
			m_Current.group = ++m_Group;
		}
		else if (_Match('}'))
		{
			m_Current.type = RtfToken::eParseType_GroupClose;
			m_Current.group = --m_Group;
		}
		else
		{
			m_Current.type = RtfToken::eParseType_Text;

			do
			{
				m_Current.value.push_back(m_Character);
			}
			while (_NextCharacter());
		}

		return true;
	}

	bool Tokenizer::_NextCharacter()
	{
		if (m_Input == nullptr || m_Input->eof())
		{
			return false;
		}

		m_Character = (char)m_Input->get();

		m_Column++;

		return !m_Input->eof();
	}

}; // namespace Rtf
}; // namespace ExLibris