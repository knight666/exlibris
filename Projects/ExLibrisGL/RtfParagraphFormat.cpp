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

#include "RtfParagraphFormat.h"

namespace ExLibris
{

	RtfParagraphFormat::RtfParagraphFormat(RtfDomDocument& a_Document)
		: m_Document(a_Document)
		, m_Specified(0)
		, m_SpaceBefore(RtfUnit(0.0f), m_Specified)
		, m_SpaceAfter(RtfUnit(0.0f), m_Specified)
		, m_AutoSpacingBefore(false, m_Specified)
		, m_AutoSpacingAfter(false, m_Specified)
		, m_SnapLineToGrid(true, m_Specified)
		, m_LineHeightRule(eLineHeightRule_Automatic, m_Specified)
		, m_LineHeight(RtfUnit(200.0f), m_Specified)
	{
	}

	RtfParagraphFormat::~RtfParagraphFormat()
	{
	}

	RtfDomDocument& RtfParagraphFormat::GetDocument() const
	{
		return m_Document;
	}

	const RtfUnit& RtfParagraphFormat::GetSpaceBefore() const
	{
		return m_SpaceBefore.Get();
	}

	void RtfParagraphFormat::SetSpaceBefore(const RtfUnit& a_Value)
	{
		m_SpaceBefore = a_Value;
	}

	const RtfUnit& RtfParagraphFormat::GetSpaceAfter() const
	{
		return m_SpaceAfter.Get();
	}

	void RtfParagraphFormat::SetSpaceAfter(const RtfUnit& a_Value)
	{
		m_SpaceAfter = a_Value;
	}

	bool RtfParagraphFormat::IsAutoSpacingBeforeEnabled() const
	{
		return m_AutoSpacingBefore.Get();
	}

	void RtfParagraphFormat::SetAutoSpacingBeforeEnabled(bool a_Value)
	{
		m_AutoSpacingBefore = a_Value;
	}

	bool RtfParagraphFormat::IsAutoSpacingAfterEnabled() const
	{
		return m_AutoSpacingAfter.Get();
	}

	void RtfParagraphFormat::SetAutoSpacingAfterEnabled(bool a_Value)
	{
		m_AutoSpacingAfter = a_Value;
	}

	bool RtfParagraphFormat::IsSnapLineToGridEnabled() const
	{
		return m_SnapLineToGrid.Get();
	}

	void RtfParagraphFormat::SetSnapLineToGridEnabled(bool a_Value)
	{
		m_SnapLineToGrid = a_Value;
	}

	RtfParagraphFormat::LineHeightRule RtfParagraphFormat::GetLineHeightRule() const
	{
		return m_LineHeightRule.Get();
	}

	void RtfParagraphFormat::SetLineHeightRule(RtfParagraphFormat::LineHeightRule a_Rule)
	{
		m_LineHeightRule = a_Rule;
	}

	const RtfUnit& RtfParagraphFormat::GetLineHeight() const
	{
		return m_LineHeight.Get();
	}

	void RtfParagraphFormat::SetLineHeight(const RtfUnit& a_Value)
	{
		m_LineHeight = a_Value;
	}

	IRtfParseable::Result RtfParagraphFormat::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		if (a_Token.value == "sb")
		{
			SetSpaceBefore(RtfUnit(RtfUnit::eType_Twips, (float)a_Token.parameter));

			return IRtfParseable::eResult_Handled;
		}
		else if (a_Token.value == "sa")
		{
			SetSpaceAfter(RtfUnit(RtfUnit::eType_Twips, (float)a_Token.parameter));

			return IRtfParseable::eResult_Handled;
		}
		else if (a_Token.value == "sbauto")
		{
			SetAutoSpacingBeforeEnabled(a_Token.parameter == 1);

			return IRtfParseable::eResult_Handled;
		}
		else if (a_Token.value == "saauto")
		{
			SetAutoSpacingAfterEnabled(a_Token.parameter == 1);

			return IRtfParseable::eResult_Handled;
		}
		else if (a_Token.value == "nosnaplinegrid")
		{
			SetSnapLineToGridEnabled(false);

			return IRtfParseable::eResult_Handled;
		}
		else if (a_Token.value == "sl")
		{
			if (a_Token.parameter < 0)
			{
				SetLineHeightRule(eLineHeightRule_Absolute);

				SetLineHeight(RtfUnit((float)-a_Token.parameter));
			}
			else if (a_Token.parameter == 0)
			{
				SetLineHeightRule(eLineHeightRule_Automatic);
			}
			else
			{
				SetLineHeightRule(eLineHeightRule_Maximum);

				SetLineHeight(RtfUnit((float)a_Token.parameter));
			}

			return IRtfParseable::eResult_Handled;
		}
		else
		{
			return IRtfParseable::eResult_Propagate;
		}
	}

}; // namespace ExLibris