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

#pragma once

#include "IRtfParseable.h"
#include "RtfCharacterEncoding.h"
#include "RtfCharacterSet.h"
#include "RtfProperty.h"

namespace ExLibris
{
	struct RtfColor;
	class RtfDomDocument;
	class RtfFont;
	struct RtfLocale;
}

namespace ExLibris
{

	class RtfTextFormat
		: public IRtfParseable
	{

		friend class RtfTextFormat;

	public:

		RtfTextFormat(RtfDomDocument& a_Document, RtfTextFormat* a_Parent = nullptr);
		~RtfTextFormat();

		RtfDomDocument& GetDocument() const;

		RtfCharacterSet GetCharacterSet() const;
		void SetCharacterSet(RtfCharacterSet a_CharacterSet);

		RtfCharacterEncoding GetCharacterEncoding() const;
		void SetCharacterEncoding(RtfCharacterEncoding a_Encoding);

		const RtfLocale* GetLocale() const;
		void SetLocale(const RtfLocale* a_Locale);

		RtfFont* GetFont() const;
		void SetFont(RtfFont* a_Font);

		float GetFontSize() const;
		void SetFontSize(float a_Size);

		RtfColor* GetBackgroundColor() const;
		void SetBackgroundColor(RtfColor* a_Color);

		RtfColor* GetForegroundColor() const;
		void SetForegroundColor(RtfColor* a_Color);

		bool GetParagraphWidowControl() const;
		void SetParagraphWidowControl(bool a_Value);

		bool IsKerningEnabled() const;
		void SetKerningEnabled(bool a_Value);

		int GetMinimumKerningSize() const;
		void SetMinimumKerningSize(int a_Size);

		void Combine(const RtfTextFormat& a_Other);

	private:

		IRtfParseable::Result _ParseCommand(RtfParserState& a_State, const RtfToken& a_Token);

	private:

		RtfDomDocument& m_Document;
		RtfTextFormat* m_Parent;
		unsigned int m_Properties;
		RtfProperty<RtfCharacterSet, 0> m_CharacterSet;
		RtfProperty<RtfCharacterEncoding, 1> m_CharacterEncoding;
		RtfProperty<const RtfLocale*, 2> m_Locale;
		RtfProperty<RtfFont*, 3> m_Font;
		RtfProperty<float, 4> m_FontSize;
		RtfProperty<RtfColor*, 5> m_BackgroundColor;
		RtfProperty<RtfColor*, 6> m_ForegroundColor;
		RtfProperty<bool, 7> m_ParagraphWidowControl;
		RtfProperty<bool, 8> m_KerningEnabled;
		RtfProperty<int, 9> m_KerningMinimumSize;

		struct ParseState;
		ParseState* m_State;

	}; // class RtfFormat

}; // namespace ExLibris