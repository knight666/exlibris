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

#include "TextParserRtf.h"

#include "Tokenizer.h"

namespace ExLibris
{

	TextParserRtf::TextParserRtf()
		: m_Tokenizer(nullptr)
		, m_Valid(false)
		, m_CharacterSet(eCharacterSet_ANSI)
	{
		m_Tokenizer = new Tokenizer(nullptr);
	}
	
	TextParserRtf::~TextParserRtf()
	{
		delete m_Tokenizer;
	}

	bool TextParserRtf::IsValid() const
	{
		return m_Valid;
	}

	TextParserRtf::CharacterSet TextParserRtf::GetCharacterSet() const
	{
		return m_CharacterSet;
	}

	void TextParserRtf::SetInput(const std::string& a_Text)
	{
		m_Input.clear();
		m_Input << a_Text;
		m_Tokenizer->SetInput(&m_Input);

		m_CommandCurrent = "";

		ParseType parsed = eParseType_Invalid;
		
		// document group

		parsed = _Parse();
		if (parsed != eParseType_GroupOpen)
		{
			m_Valid = false;
			return;
		}

		parsed = _Parse();
		if (parsed != eParseType_Command || m_CommandCurrent != "rtf1")
		{
			m_Valid = false;
			return;
		}

		// character set

		parsed = _Parse();
		if (parsed != eParseType_Command)
		{
			m_Valid = false;
			return;
		}

		if (m_CommandCurrent == "ansi")
		{
			m_CharacterSet = eCharacterSet_ANSI;
		}
		else if (m_CommandCurrent == "mac")
		{
			m_CharacterSet = eCharacterSet_AppleMacintosh;
		}
		else if (m_CommandCurrent == "pc")
		{
			m_CharacterSet = eCharacterSet_IBMPCCodePage437;
		}
		else
		{
			m_CharacterSet = eCharacterSet_Invalid;
			m_Valid = false;

			return;
		}

		m_Valid = true;
	}

	bool TextParserRtf::ReadToken()
	{
		return false;
	}

	TextParserRtf::ParseType TextParserRtf::_Parse()
	{
		ParseType result = eParseType_Invalid;

		if (!m_Tokenizer->ReadToken())
		{
			return result;
		}

		const Token& tk = m_Tokenizer->GetCurrentToken();
		if (tk.type == Token::eType_Symbol)
		{
			switch (tk.text[0])
			{

			case '{':
				{
					m_GroupIndex++;

					result = eParseType_GroupOpen;

				} break;

			case '}':
				{
					m_GroupIndex--;

					result = eParseType_GroupClose;

				} break;

			case '\\':
				{
					if (!m_Tokenizer->ReadToken())
					{
						result = eParseType_Invalid;

						break;
					}

					if (tk.type == Token::eType_Identifier)
					{
						m_CommandCurrent = tk.text;

						result = eParseType_Command;
					}
					
				} break;
			}
		}

		return result;
	}

}; // namespace ExLibris