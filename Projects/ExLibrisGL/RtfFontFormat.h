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
#include "RtfDomDocument.h"

namespace ExLibris {
namespace Rtf {

	class FontFormat
		: public IRtfParseable
	{

	public:

		FontFormat(RtfDomDocument& a_Document);
		~FontFormat();

		RtfDomDocument& GetDocument() const;

		void Reset();

		const RtfLocale* GetLocale(TextLanguage a_Language = eTextLanguage_Default, SpellingCheck a_Checked = eSpellingCheck_Enabled) const;
		void SetLocale(TextLanguage a_Language, SpellingCheck a_Checked, const RtfLocale* a_Locale);

		bool IsBold() const;
		void SetBold(bool a_Value);

		bool IsItalic() const;
		void SetItalic(bool a_Value);

		bool GetProofing() const;
		void SetProofing(bool a_Proofing);

	private:

		IRtfParseable::Result _ParseCommand(RtfParserState& a_State, const RtfToken& a_Token);

	private:

		RtfDomDocument& m_Document;
		bool m_Bold;
		bool m_Italic;
		const RtfLocale* m_LocalesChecked[3];
		const RtfLocale* m_LocalesUnchecked[3];
		bool m_Proofing;

	}; // class FontFormat

}; // namespace Rtf
}; // namespace ExLibris