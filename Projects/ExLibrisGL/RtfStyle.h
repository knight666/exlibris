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

#include "RtfTextFormat.h"

namespace ExLibris
{
	struct RtfColor;
}

namespace ExLibris
{

	class RtfStyle
	{

	public:

		RtfStyle();
		~RtfStyle();

		RtfStyle* GetNextParagraphStyle() const;
		void SetNextParagraphStyle(RtfStyle* a_Style);

		RtfColor* GetColorBackground() const;
		void SetColorBackground(RtfColor* a_Color);

		RtfColor* GetColorForeground() const;
		void SetColorForeground(RtfColor* a_Color);

		RtfTextFormat TextFormat;

	private:

		RtfStyle* m_StyleNextParagraph;
		RtfColor* m_ColorBackground;
		RtfColor* m_ColorForeground;

	}; // class RtfStyle

}; // namespace ExLibris