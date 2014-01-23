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
		, m_Specified(0)
		, m_Font(nullptr, m_Specified)
		, m_FontSize(12.0f, m_Specified)
		, m_Locale(nullptr, m_Specified)
		, m_Encoding(Rtf::eCharacterEncoding_SingleByteLowAnsi, m_Specified)
		, m_Bold(false, m_Specified)
		, m_Italic(false, m_Specified)
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
	}

	RtfFont* RtfAssociatedProperties::GetFont() const
	{
		return m_Font.Get();
	}

	void RtfAssociatedProperties::SetFont(RtfFont* a_Font)
	{
		m_Font = a_Font;
	}

	float RtfAssociatedProperties::GetFontSize() const
	{
		return m_FontSize.Get();
	}

	void RtfAssociatedProperties::SetFontSize(float a_Size)
	{
		m_FontSize = a_Size;
	}

	const RtfLocale* RtfAssociatedProperties::GetLocale() const
	{
		return m_Locale.Get();
	}

	void RtfAssociatedProperties::SetLocale(const RtfLocale* a_Locale)
	{
		m_Locale = a_Locale;
	}

	Rtf::CharacterEncoding RtfAssociatedProperties::GetCharacterEncoding() const
	{
		return m_Encoding.Get();
	}

	void RtfAssociatedProperties::SetCharacterEncoding(Rtf::CharacterEncoding a_Encoding)
	{
		m_Encoding = a_Encoding;
	}

	bool RtfAssociatedProperties::IsBold() const
	{
		return m_Bold.Get();
	}

	void RtfAssociatedProperties::SetBold(bool a_Value)
	{
		m_Bold = a_Value;
	}

	bool RtfAssociatedProperties::IsItalic() const
	{
		return m_Italic.Get();
	}

	void RtfAssociatedProperties::SetItalic(bool a_Italic)
	{
		m_Italic = a_Italic;
	}

	void RtfAssociatedProperties::Combine(const RtfAssociatedProperties& a_Other)
	{
		m_Font = a_Other.m_Font;
		m_FontSize = a_Other.m_FontSize;
		m_Locale = a_Other.m_Locale;
		m_Encoding = a_Other.m_Encoding;
		m_Bold = a_Other.m_Bold;
		m_Italic = a_Other.m_Italic;
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
			if (a_Token.parameter < 0)
			{
				return eResult_Invalid;
			}

			float font_size = (float)a_Token.parameter / 2.0f;
			SetFontSize(font_size);

			return eResult_Handled;
		}
		else if (a_Token.value == "alang")
		{
			if (a_Token.parameter < 0 || m_Document.GetWorld() == nullptr)
			{
				return eResult_Invalid;
			}

			const RtfLocale* locale = m_Document.GetWorld()->GetLocaleByIdentifier(a_Token.parameter);
			if (locale == nullptr)
			{
				return eResult_Invalid;
			}

			SetLocale(locale);

			return eResult_Handled;
		}
		else
		{
			return eResult_Propagate;
		}
	}

}; // namespace ExLibris