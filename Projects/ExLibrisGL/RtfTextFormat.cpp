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
#include "RtfFontTable.h"
#include "RtfDomDocument.h"
#include "RtfWorld.h"

namespace ExLibris
{

	struct RtfTextFormat::ParseState
	{
		ParseState()
			: character_encoding_found(false)
		{
		}

		bool character_encoding_found;
	};

	RtfTextFormat::RtfTextFormat(RtfDomDocument& a_Document, RtfTextFormat* a_Parent /*= nullptr*/)
		: m_Document(a_Document)
		, m_Parent(a_Parent)
		, m_Properties(0)
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
		, m_State(new ParseState)
	{
	}

	RtfTextFormat::RtfTextFormat(const RtfTextFormat& a_Other)
		: m_Document(a_Other.GetDocument())
		, m_Properties(a_Other.m_Properties)
		, m_CharacterSet(a_Other.GetCharacterSet())
		, m_CharacterEncoding(a_Other.GetCharacterEncoding())
		, m_Font(a_Other.GetFont())
		, m_FontSize(a_Other.GetFontSize())
		, m_Locale(a_Other.GetLocale())
		, m_BackgroundColor(a_Other.GetBackgroundColor())
		, m_ForegroundColor(a_Other.GetForegroundColor())
		, m_ParagraphWidowControl(a_Other.GetParagraphWidowControl())
		, m_KerningEnabled(a_Other.IsKerningEnabled())
		, m_KerningMinimumSize(a_Other.GetMinimumKerningSize())
		, m_State(new ParseState)
	{
	}

	RtfTextFormat::~RtfTextFormat()
	{
		delete m_State;
	}

	RtfDomDocument& RtfTextFormat::GetDocument() const
	{
		return m_Document;
	}

	RtfCharacterSet RtfTextFormat::GetCharacterSet() const
	{
		if ((m_Properties & eProperty_CharacterSet) == 0 && m_Parent != nullptr)
		{
			return m_Parent->GetCharacterSet();
		}
		else
		{
			return m_CharacterSet;
		}
	}

	void RtfTextFormat::SetCharacterSet(RtfCharacterSet a_CharacterSet)
	{
		m_CharacterSet = a_CharacterSet;

		m_Properties |= eProperty_CharacterSet;
	}

	RtfCharacterEncoding RtfTextFormat::GetCharacterEncoding() const
	{
		if ((m_Properties & eProperty_CharacterEncoding) == 0 && m_Parent != nullptr)
		{
			return m_Parent->GetCharacterEncoding();
		}
		else
		{
			return m_CharacterEncoding;
		}
	}

	void RtfTextFormat::SetCharacterEncoding(RtfCharacterEncoding a_Encoding)
	{
		m_CharacterEncoding = a_Encoding;

		m_Properties |= eProperty_CharacterEncoding;
	}

	const RtfLocale* RtfTextFormat::GetLocale() const
	{
		if ((m_Properties & eProperty_Locale) == 0 && m_Parent != nullptr)
		{
			return m_Parent->GetLocale();
		}
		else
		{
			return m_Locale;
		}
	}

	void RtfTextFormat::SetLocale(const RtfLocale* a_Locale)
	{
		m_Locale = a_Locale;

		m_Properties |= eProperty_Locale;
	}

	RtfFont* RtfTextFormat::GetFont() const
	{
		if ((m_Properties & eProperty_Font) == 0 && m_Parent != nullptr)
		{
			return m_Parent->GetFont();
		}
		else
		{
			return m_Font;
		}
	}

	void RtfTextFormat::SetFont(RtfFont* a_Font)
	{
		m_Font = a_Font;

		m_Properties |= eProperty_Font;
	}

	float RtfTextFormat::GetFontSize() const
	{
		if ((m_Properties & eProperty_FontSize) == 0 && m_Parent != nullptr)
		{
			return m_Parent->GetFontSize();
		}
		else
		{
			return m_FontSize;
		}
	}

	void RtfTextFormat::SetFontSize(float a_Size)
	{
		m_FontSize = a_Size;

		m_Properties |= eProperty_FontSize;
	}

	RtfColor* RtfTextFormat::GetBackgroundColor() const
	{
		if ((m_Properties & eProperty_BackgroundColor) == 0 && m_Parent != nullptr)
		{
			return m_Parent->GetBackgroundColor();
		}
		else
		{
			return m_BackgroundColor;
		}
	}

	void RtfTextFormat::SetBackgroundColor(RtfColor* a_Color)
	{
		m_BackgroundColor = a_Color;

		m_Properties |= eProperty_BackgroundColor;
	}

	RtfColor* RtfTextFormat::GetForegroundColor() const
	{
		if ((m_Properties & eProperty_ForegroundColor) == 0 && m_Parent != nullptr)
		{
			return m_Parent->GetForegroundColor();
		}
		else
		{
			return m_ForegroundColor;
		}
	}

	void RtfTextFormat::SetForegroundColor(RtfColor* a_Color)
	{
		m_ForegroundColor = a_Color;

		m_Properties |= eProperty_ForegroundColor;
	}

	bool RtfTextFormat::GetParagraphWidowControl() const
	{
		if ((m_Properties & eProperty_ParagraphWidowControl) == 0 && m_Parent != nullptr)
		{
			return m_Parent->GetParagraphWidowControl();
		}
		else
		{
			return m_ParagraphWidowControl;
		}
	}

	void RtfTextFormat::SetParagraphWidowControl(bool a_Value)
	{
		m_ParagraphWidowControl = a_Value;

		m_Properties |= eProperty_ParagraphWidowControl;
	}

	bool RtfTextFormat::IsKerningEnabled() const
	{
		if ((m_Properties & eProperty_KerningEnabled) == 0 && m_Parent != nullptr)
		{
			return m_Parent->IsKerningEnabled();
		}
		else
		{
			return m_KerningEnabled;
		}
	}

	void RtfTextFormat::SetKerningEnabled(bool a_Value)
	{
		m_KerningEnabled = a_Value;

		m_Properties |= eProperty_KerningEnabled;
	}

	int RtfTextFormat::GetMinimumKerningSize() const
	{
		if ((m_Properties & eProperty_MinimumKerningSize) == 0 && m_Parent != nullptr)
		{
			return m_Parent->GetMinimumKerningSize();
		}
		else
		{
			return m_KerningMinimumSize;
		}
	}

	void RtfTextFormat::SetMinimumKerningSize(int a_Size)
	{
		m_KerningMinimumSize = a_Size;

		m_Properties |= eProperty_MinimumKerningSize;
	}

	IRtfParseable::Result RtfTextFormat::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		// character set

		if (a_Token.value == "ansi")
		{
			if (m_State->character_encoding_found)
			{
				return eResult_Invalid;
			}

			SetCharacterSet(eRtfCharacterSet_Ansi);

			m_State->character_encoding_found = true;

			return eResult_Handled;
		}
		else if (a_Token.value == "mac")
		{
			if (m_State->character_encoding_found)
			{
				return eResult_Invalid;
			}

			SetCharacterSet(eRtfCharacterSet_AppleMacintoshRoman);

			m_State->character_encoding_found = true;

			return eResult_Handled;
		}
		else if (a_Token.value == "pc")
		{
			if (m_State->character_encoding_found)
			{
				return eResult_Invalid;
			}

			SetCharacterSet(eRtfCharacterSet_IbmPcCodePage437);

			m_State->character_encoding_found = true;

			return eResult_Handled;
		}
		else if (a_Token.value == "pca")
		{
			if (m_State->character_encoding_found)
			{
				return eResult_Invalid;
			}

			SetCharacterSet(eRtfCharacterSet_Oem);

			m_State->character_encoding_found = true;

			return eResult_Handled;
		}

		// font

		else if (a_Token.value == "f")
		{
			if (a_Token.parameter < 0)
			{
				return eResult_Invalid;
			}

			RtfFont* font = m_Document.GetFontTable()->GetFont(a_Token.parameter);
			SetFont(font);

			return eResult_Handled;
		}
		
		// font size

		else if (a_Token.value == "fs")
		{
			if (a_Token.parameter < 0)
			{
				return eResult_Invalid;
			}

			float font_size = (float)a_Token.parameter / 2.0f;
			SetFontSize(font_size);

			return eResult_Handled;
		}

		// kerning

		else if (a_Token.value == "kerning")
		{
			bool kerning_enabled = (a_Token.parameter == 1);
			SetKerningEnabled(kerning_enabled);

			return eResult_Handled;
		}

		// language

		else if (a_Token.value == "lang" || a_Token.value == "langfe")
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

		// foreground color

		else if (a_Token.value == "cf")
		{
			if (a_Token.parameter < 0)
			{
				return eResult_Invalid;
			}

			RtfColor* color = m_Document.GetColorTable()->GetColor(a_Token.parameter);
			SetForegroundColor(color);

			return eResult_Handled;
		}

		// paragraph widow control

		else if (a_Token.value == "nowidctlpar")
		{
			SetParagraphWidowControl(false);

			return eResult_Handled;
		}
		else
		{
			return eResult_Propagate;
		}
	}

}; // namespace ExLibris