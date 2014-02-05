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
	{
		Reset();
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
		m_LocalesChecked[0] = nullptr;
		m_LocalesChecked[1] = nullptr;
		m_LocalesChecked[2] = nullptr;
		m_LocalesUnchecked[0] = nullptr;
		m_LocalesUnchecked[1] = nullptr;
		m_LocalesUnchecked[2] = nullptr;
		SetBold(false);
		SetItalic(false);
		SetProofing(false);
	}

	const RtfLocale* FontFormat::GetLocale(TextLanguage a_Language /*= eTextLanguage_Default*/, SpellingCheck a_Checked /*= eSpellingCheck_Enabled*/) const
	{
		if (a_Checked == eSpellingCheck_Enabled)
		{
			return m_LocalesChecked[a_Language];
		}
		else
		{
			return m_LocalesUnchecked[a_Language];
		}
	}

	void FontFormat::SetLocale(TextLanguage a_Language, SpellingCheck a_Checked, const RtfLocale* a_Locale)
	{
		if (a_Locale == nullptr)
		{
			return;
		}

		if (a_Checked == eSpellingCheck_Enabled)
		{
			m_LocalesChecked[a_Language] = a_Locale;

			SetProofing(a_Locale->identifier != 1024);
		}
		else
		{
			m_LocalesUnchecked[a_Language] = a_Locale;
		}
	}

	bool FontFormat::IsBold() const
	{
		return m_Bold;
	}

	void FontFormat::SetBold(bool a_Value)
	{
		m_Bold = a_Value;
	}

	bool FontFormat::IsItalic() const
	{
		return m_Italic;
	}

	void FontFormat::SetItalic(bool a_Value)
	{
		m_Italic = a_Value;
	}

	bool FontFormat::GetProofing() const
	{
		return m_Proofing;
	}
	
	void FontFormat::SetProofing(bool a_Proofing)
	{
		m_Proofing = a_Proofing;
	}

	IRtfParseable::Result FontFormat::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		if (a_Token.value == "plain")
		{
			Reset();

			return IRtfParseable::eResult_Handled;
		}
		else if (a_Token.value == "lang" || a_Token.value == "langfe" || a_Token.value == "langnp" || a_Token.value == "langfenp")
		{
			if (m_Document.GetWorld() == nullptr || a_Token.parameter < 0)
			{
				return IRtfParseable::eResult_Invalid;
			}

			const RtfLocale* locale = m_Document.GetWorld()->GetLocaleByIdentifier(a_Token.parameter);
			if (locale == nullptr)
			{
				return IRtfParseable::eResult_Invalid;
			}

			if (a_Token.value == "langfe")
			{
				SetLocale(eTextLanguage_EastAsian, eSpellingCheck_Enabled, locale);
			}
			else if (a_Token.value == "langnp")
			{
				SetLocale(eTextLanguage_Default, eSpellingCheck_Disabled, locale);
			}
			else if (a_Token.value == "langfenp")
			{
				SetLocale(eTextLanguage_EastAsian, eSpellingCheck_Disabled, locale);
			}
			else
			{
				SetLocale(eTextLanguage_Default, eSpellingCheck_Enabled, locale);
			}

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
		else if (a_Token.value == "noproof")
		{
			SetProofing(false);

			return IRtfParseable::eResult_Handled;
		}
		else
		{
			return IRtfParseable::eResult_Propagate;
		}
	}

}; // namespace Rtf
}; // namespace ExLibris