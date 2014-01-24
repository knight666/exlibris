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
#include "RtfProperty.h"
#include "RtfTypes.h"
#include "RtfUnit.h"

namespace ExLibris
{
	class RtfDomDocument;
}

namespace ExLibris
{

	class RtfParagraphFormat
		: public IRtfParseable
	{

	public:

		RtfParagraphFormat(RtfDomDocument& a_Document);
		~RtfParagraphFormat();

		RtfDomDocument& GetDocument() const;

		// General

		bool IsKeepIntactEnabled() const;
		void SetKeepIntactEnabled(bool a_Value);

		bool IsKeepWithNextEnabled() const;
		void SetKeepWithNextEnabled(bool a_Value);

		bool IsLineNumberingEnabled() const;
		void SetLineNumberingEnabled(bool a_Value);

		// Spacing

		const RtfUnit& GetSpaceBefore() const;
		void SetSpaceBefore(const RtfUnit& a_Value);

		const RtfUnit& GetSpaceAfter() const;
		void SetSpaceAfter(const RtfUnit& a_Value);

		bool IsAutoSpacingBeforeEnabled() const;
		void SetAutoSpacingBeforeEnabled(bool a_Value);

		bool IsAutoSpacingAfterEnabled() const;
		void SetAutoSpacingAfterEnabled(bool a_Value);

		bool IsSnapLineToGridEnabled() const;
		void SetSnapLineToGridEnabled(bool a_Value);

		Rtf::LineHeightRule GetLineHeightRule() const;
		void SetLineHeightRule(Rtf::LineHeightRule a_Rule);

		const RtfUnit& GetLineHeight() const;
		void SetLineHeight(const RtfUnit& a_Value);

	private:

		IRtfParseable::Result _ParseCommand(RtfParserState& a_State, const RtfToken& a_Token);

	private:

		RtfDomDocument& m_Document;
		unsigned int m_SpecifiedGeneral;
		RtfProperty<bool, 0> m_KeepIntactEnabled;
		RtfProperty<bool, 1> m_KeepWithNextEnabled;
		RtfProperty<bool, 2> m_LineNumberingEnabled;
		unsigned int m_SpecifiedSpacing;
		RtfProperty<RtfUnit, 0> m_SpaceBefore;
		RtfProperty<RtfUnit, 1> m_SpaceAfter;
		RtfProperty<bool, 2> m_AutoSpacingBefore;
		RtfProperty<bool, 3> m_AutoSpacingAfter;
		RtfProperty<bool, 4> m_SnapLineToGrid;
		RtfProperty<Rtf::LineHeightRule, 5> m_LineHeightRule;
		RtfProperty<RtfUnit, 6> m_LineHeight;

	}; // class RtfParagraphFormat

}; // namespace ExLibris