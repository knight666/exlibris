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
#include "RtfProperty.h"

namespace ExLibris
{
	class RtfDomDocument;
	class RtfFont;
	struct RtfLocale;
	class RtfTextFormat;
}

namespace ExLibris
{

	class RtfAssociatedProperties
		: public IRtfParseable
	{

		friend class RtfAssociatedProperties;

	public:

		RtfAssociatedProperties(RtfDomDocument& a_Document);
		~RtfAssociatedProperties();

		void FromTextFormat(const RtfTextFormat& a_TextFormat);

		RtfFont* GetFont() const;
		void SetFont(RtfFont* a_Font);

		float GetFontSize() const;
		void SetFontSize(float a_Size);

		const RtfLocale* GetLocale() const;
		void SetLocale(const RtfLocale* a_Locale);

		RtfCharacterEncoding GetCharacterEncoding() const;
		void SetCharacterEncoding(RtfCharacterEncoding a_Encoding);

		bool IsBold() const;
		void SetBold(bool a_Value);

		bool IsItalic() const;
		void SetItalic(bool a_Italic);

		void Combine(const RtfAssociatedProperties& a_Other);

	private:

		IRtfParseable::Result _ParseCommand(RtfParserState& a_State, const RtfToken& a_Token);

	private:

		RtfDomDocument& m_Document;
		unsigned int m_Specified;
		RtfProperty<RtfFont*, 0> m_Font;
		RtfProperty<float, 1> m_FontSize;
		RtfProperty<const RtfLocale*, 2> m_Locale;
		RtfProperty<RtfCharacterEncoding, 3> m_Encoding;
		RtfProperty<bool, 4> m_Bold;
		RtfProperty<bool, 5> m_Italic;

	}; // class RtfAssociatedProperties

}; // namespace ExLibris