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

#include "RtfTextFormat.h"

#include "RtfColorTable.h"
#include "RtfDomDocument.h"

namespace ExLibris
{

	RtfTextFormat::RtfTextFormat()
		: m_Document(nullptr)
		, m_CharacterSet(eRtfCharacterSet_Invalid)
		, m_CharacterEncoding(eRtfCharacterEncoding_SingleByteLowAnsi)
		, m_Font(nullptr)
		, m_FontSize(12.0f)
		, m_Locale(nullptr)
		, m_BackgroundColor(nullptr)
		, m_ForegroundColor(nullptr)
		, m_ParagraphWidowControl(true)
		, m_KerningEnabled(true)
		, m_KerningMinimumSize(1)
	{
		if (m_Document != nullptr)
		{
			m_BackgroundColor = m_Document->GetColorTable()->GetDefaultColor();
			m_ForegroundColor = m_BackgroundColor;

			m_ParagraphWidowControl = m_Document->GetWidowControl();
		}
	}

	RtfCharacterSet RtfTextFormat::GetCharacterSet() const
	{
		return m_CharacterSet;
	}

	void RtfTextFormat::SetCharacterSet(RtfCharacterSet a_CharacterSet)
	{
		m_CharacterSet = a_CharacterSet;
	}

	RtfCharacterEncoding RtfTextFormat::GetCharacterEncoding() const
	{
		return m_CharacterEncoding;
	}

	void RtfTextFormat::SetCharacterEncoding(RtfCharacterEncoding a_Encoding)
	{
		m_CharacterEncoding = a_Encoding;
	}

	const RtfLocale* RtfTextFormat::GetLocale() const
	{
		return m_Locale;
	}

	void RtfTextFormat::SetLocale(const RtfLocale* a_Locale)
	{
		m_Locale = a_Locale;
	}

	RtfFont* RtfTextFormat::GetFont() const
	{
		return m_Font;
	}

	void RtfTextFormat::SetFont(RtfFont* a_Font)
	{
		m_Font = a_Font;
	}

	float RtfTextFormat::GetFontSize() const
	{
		return m_FontSize;
	}

	void RtfTextFormat::SetFontSize(float a_Size)
	{
		m_FontSize = a_Size;
	}

	RtfColor* RtfTextFormat::GetBackgroundColor() const
	{
		return m_BackgroundColor;
	}

	void RtfTextFormat::SetBackgroundColor(RtfColor* a_Color)
	{
		m_BackgroundColor = a_Color;
	}

	RtfColor* RtfTextFormat::GetForegroundColor() const
	{
		return m_ForegroundColor;
	}

	void RtfTextFormat::SetForegroundColor(RtfColor* a_Color)
	{
		m_ForegroundColor = a_Color;
	}

	bool RtfTextFormat::GetParagraphWidowControl() const
	{
		return m_ParagraphWidowControl;
	}

	void RtfTextFormat::SetParagraphWidowControl(bool a_Value)
	{
		m_ParagraphWidowControl = a_Value;
	}

	bool RtfTextFormat::IsKerningEnabled() const
	{
		return m_KerningEnabled;
	}

	void RtfTextFormat::SetKerningEnabled(bool a_Value)
	{
		m_KerningEnabled = a_Value;
	}

	int RtfTextFormat::GetMinimumKerningSize() const
	{
		return m_KerningMinimumSize;
	}

	void RtfTextFormat::SetMinimumKerningSize(int a_Size)
	{
		m_KerningMinimumSize = a_Size;
	}

}; // namespace ExLibris