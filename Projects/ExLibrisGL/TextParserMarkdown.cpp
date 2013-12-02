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

#include "TextParserMarkdown.h"

namespace ExLibris
{

	TextParserMarkdown::TextParserMarkdown()
		: m_EscapeCharacter(false)
	{
	}
	
	TextParserMarkdown::~TextParserMarkdown()
	{
	}

	void TextParserMarkdown::SetInput(const std::string& a_Text)
	{
		m_Text = a_Text;
		m_TextCursor = m_Text.begin();

		m_EscapeCharacter = false;

		TextParserToken clear;
		m_Token = clear;
	}

	bool TextParserMarkdown::ReadToken()
	{
		if (!_ReadNextCodepoint())
		{
			return false;
		}

		m_Token.codepoint = 0;
		m_Token.changes = TextParserToken::eChanged_None;

		switch (m_CodepointCurrent)
		{

		case '\\':
			{
				if (m_EscapeCharacter)
				{
					m_Token.codepoint = m_CodepointCurrent;

					m_EscapeCharacter = false;
				}
				else
				{
					m_EscapeCharacter = true;

					return ReadToken();
				}

			} break;

		case '*':
			{
				if (m_EscapeCharacter)
				{
					m_Token.codepoint = m_CodepointCurrent;
				}
				else
				{
					if (_NextCodepoint() == '*')
					{
						m_TextCursor++;

						_ToggleBold();
					}
					else
					{
						_ToggleItalic();
					}
				}

				m_EscapeCharacter = false;

			} break;

		default:
			{
				m_Token.codepoint = m_CodepointCurrent;

			} break;

		}

		return true;
	}

	const TextParserToken& TextParserMarkdown::GetToken() const
	{
		return m_Token;
	}

	int TextParserMarkdown::_NextCodepoint()
	{
		if (m_TextCursor == m_Text.end())
		{
			return 0;
		}

		return (int)*m_TextCursor;
	}

	bool TextParserMarkdown::_ReadNextCodepoint()
	{
		if (m_TextCursor == m_Text.end())
		{
			return false;
		}

		m_CodepointCurrent = (int)*m_TextCursor++;

		return true;
	}

	void TextParserMarkdown::_ToggleItalic()
	{
		m_Token.style = (m_Token.style == eStyle_None) ? eStyle_Italicized : eStyle_None;
		m_Token.changes |= TextParserToken::eChanged_Style;
	}

	void TextParserMarkdown::_ToggleBold()
	{
		m_Token.weight = (m_Token.weight == eWeight_Normal) ? eWeight_Bold : eWeight_Normal;
		m_Token.changes |= TextParserToken::eChanged_Weight;
	}

}; // namespace ExLibris