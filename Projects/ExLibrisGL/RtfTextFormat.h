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

	private:

		enum Property
		{
			eProperty_CharacterSet = (1 << 0),
			eProperty_CharacterEncoding = (1 << 1),
			eProperty_Locale = (1 << 2),
			eProperty_Font = (1 << 3),
			eProperty_FontSize = (1 << 4),
			eProperty_BackgroundColor = (1 << 5),
			eProperty_ForegroundColor = (1 << 6),
			eProperty_ParagraphWidowControl = (1 << 7),
			eProperty_KerningEnabled = (1 << 8),
			eProperty_MinimumKerningSize = (1 << 9),
		};

	public:

		RtfTextFormat(RtfDomDocument& a_Document);
		RtfTextFormat(const RtfTextFormat& a_Other);
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

	private:

		IRtfParseable::Result _ParseCommand(RtfParserState& a_State, const RtfToken& a_Token);

	private:

		RtfDomDocument& m_Document;
		unsigned int m_Properties;
		RtfCharacterSet m_CharacterSet;
		RtfCharacterEncoding m_CharacterEncoding;
		const RtfLocale* m_Locale;
		RtfFont* m_Font;
		float m_FontSize;
		RtfColor* m_BackgroundColor;
		RtfColor* m_ForegroundColor;
		bool m_ParagraphWidowControl;
		bool m_KerningEnabled;
		int m_KerningMinimumSize;

		struct ParseState;
		ParseState* m_State;

	}; // class RtfFormat

}; // namespace ExLibris