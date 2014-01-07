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

#include "RtfStyle.h"

namespace ExLibris
{

	RtfStyle::RtfStyle()
		: m_ColorBackground(nullptr)
		, m_ColorForeground(nullptr)
	{
		m_StyleNextParagraph = this;
	}

	RtfStyle::~RtfStyle()
	{
	}

	RtfStyle* RtfStyle::GetNextParagraphStyle() const
	{
		return m_StyleNextParagraph;
	}

	void RtfStyle::SetNextParagraphStyle(RtfStyle* a_Style)
	{
		m_StyleNextParagraph = a_Style;
	}

	RtfColor* RtfStyle::GetColorBackground() const
	{
		return m_ColorBackground;
	}

	void RtfStyle::SetColorBackground(RtfColor* a_Color)
	{
		m_ColorBackground = a_Color;
	}

	RtfColor* RtfStyle::GetColorForeground() const
	{
		return m_ColorForeground;
	}

	void RtfStyle::SetColorForeground(RtfColor* a_Color)
	{
		m_ColorForeground = a_Color;
	}

}; // namespace ExLibris