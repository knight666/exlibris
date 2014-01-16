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

#include "RtfAssociatedProperties.h"

#include "RtfDomDocument.h"
#include "RtfFontTable.h"
#include "RtfTextFormat.h"
#include "RtfWorld.h"

namespace ExLibris
{

	RtfAssociatedProperties::RtfAssociatedProperties(RtfDomDocument& a_Document)
		: m_Document(a_Document)
		, m_Font(nullptr)
		, m_FontSize(12.0f)
		, m_Locale(nullptr)
		, m_Encoding(eRtfCharacterEncoding_SingleByteLowAnsi)
		, m_Bold(false)
		, m_Italic(false)
		, m_Specified(0)
	{
	}

	RtfAssociatedProperties::~RtfAssociatedProperties()
	{
	}

	void RtfAssociatedProperties::FromTextFormat(const RtfTextFormat& a_TextFormat)
	{
		m_Font = a_TextFormat.GetFont();
		m_FontSize = a_TextFormat.GetFontSize();
		m_Locale = a_TextFormat.GetLocale();
		m_Encoding = a_TextFormat.GetCharacterEncoding();

		m_Specified = eSpecified_Font | eSpecified_FontSize | eSpecified_Locale | eSpecified_CharacterEncoding;
	}

	RtfFont* RtfAssociatedProperties::GetFont() const
	{
		return m_Font;
	}

	void RtfAssociatedProperties::SetFont(RtfFont* a_Font)
	{
		m_Font = a_Font;
		m_Specified |= eSpecified_Font;
	}

	float RtfAssociatedProperties::GetFontSize() const
	{
		return m_FontSize;
	}

	void RtfAssociatedProperties::SetFontSize(float a_Size)
	{
		m_FontSize = a_Size;
		m_Specified |= eSpecified_FontSize;
	}

	const RtfLocale* RtfAssociatedProperties::GetLocale() const
	{
		return m_Locale;
	}

	void RtfAssociatedProperties::SetLocale(const RtfLocale* a_Locale)
	{
		m_Locale = a_Locale;
		m_Specified |= eSpecified_Locale;
	}

	RtfCharacterEncoding RtfAssociatedProperties::GetCharacterEncoding() const
	{
		return m_Encoding;
	}

	void RtfAssociatedProperties::SetCharacterEncoding(RtfCharacterEncoding a_Encoding)
	{
		m_Encoding = a_Encoding;
		m_Specified |= eSpecified_CharacterEncoding;
	}

	bool RtfAssociatedProperties::IsBold() const
	{
		return m_Bold;
	}

	void RtfAssociatedProperties::SetBold(bool a_Value)
	{
		m_Bold = a_Value;
		m_Specified |= eSpecified_Bold;
	}

	bool RtfAssociatedProperties::IsItalic() const
	{
		return m_Italic;
	}

	void RtfAssociatedProperties::SetItalic(bool a_Italic)
	{
		m_Italic = a_Italic;
		m_Specified |= eSpecified_Italic;
	}

	void RtfAssociatedProperties::Combine(const RtfAssociatedProperties& a_Other)
	{
		if (a_Other.m_Specified & eSpecified_Font)
		{
			SetFont(a_Other.GetFont());
		}
		if (a_Other.m_Specified & eSpecified_FontSize)
		{
			SetFontSize(a_Other.GetFontSize());
		}
		if (a_Other.m_Specified & eSpecified_Locale)
		{
			SetLocale(a_Other.GetLocale());
		}
		if (a_Other.m_Specified & eSpecified_CharacterEncoding)
		{
			SetCharacterEncoding(a_Other.GetCharacterEncoding());
		}
		if (a_Other.m_Specified & eSpecified_Bold)
		{
			SetBold(a_Other.IsBold());
		}
		if (a_Other.m_Specified & eSpecified_Italic)
		{
			SetItalic(a_Other.IsItalic());
		}
	}

	IRtfParseable::Result RtfAssociatedProperties::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		if (a_Token.value == "af")
		{
			if (a_Token.parameter < 0)
			{
				return eResult_Invalid;
			}

			RtfFont* font = m_Document.GetFontTable()->GetFont(a_Token.parameter);
			SetFont(font);

			return eResult_Handled;
		}
		else if (a_Token.value == "afs")
		{
			float font_size = (float)a_Token.parameter / 2.0f;
			SetFontSize(font_size);

			return eResult_Handled;
		}
		else if (a_Token.value == "alang")
		{
			if (a_Token.parameter < 0)
			{
				return eResult_Invalid;
			}

			const RtfLocale* locale = m_Document.GetWorld()->GetLocaleByIdentifier(a_Token.parameter);
			SetLocale(locale);

			return eResult_Handled;
		}
		else
		{
			return eResult_Propagate;
		}
	}

}; // namespace ExLibris