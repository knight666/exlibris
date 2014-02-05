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

#include "RtfFontFormat.h"

namespace ExLibris {
namespace Rtf {

	FontFormat::FontFormat(RtfDomDocument& a_Document)
		: m_Document(a_Document)
		, m_Specified(0)
		, m_Bold(false, m_Specified)
		, m_Italic(false, m_Specified)
	{
	}

	FontFormat::~FontFormat()
	{
	}

	RtfDomDocument& FontFormat::GetDocument() const
	{
		return m_Document;
	}

	void FontFormat::Reset()
	{
		SetBold(false);
		SetItalic(false);
	}

	bool FontFormat::IsBold() const
	{
		return m_Bold.Get();
	}

	void FontFormat::SetBold(bool a_Value)
	{
		m_Bold = a_Value;
	}

	bool FontFormat::IsItalic() const
	{
		return m_Italic.Get();
	}

	void FontFormat::SetItalic(bool a_Value)
	{
		m_Italic = a_Value;
	}

	IRtfParseable::Result FontFormat::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		if (a_Token.value == "plain")
		{
			Reset();

			return IRtfParseable::eResult_Handled;
		}
		else if (a_Token.value == "i")
		{
			SetItalic(!(a_Token.parameter == 0));

			return IRtfParseable::eResult_Handled;
		}
		else if (a_Token.value == "b")
		{
			SetBold(!(a_Token.parameter == 0));

			return IRtfParseable::eResult_Handled;
		}
		else
		{
			return IRtfParseable::eResult_Propagate;
		}
	}

}; // namespace Rtf
}; // namespace ExLibris