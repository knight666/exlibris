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

#include "IGlyphProvider.h"

namespace ExLibris
{

	class Face
	{
	
	public:
	
		Face(IGlyphProvider& a_Provider, const FontMetrics& a_Metrics);
		~Face();

		Family* GetFamily() const;

		Weight GetWeight() const;

		Style GetStyle() const;

		float GetSize() const;
		float GetLineHeight() const;
		float GetAscent() const;
		float GetDescent() const;

		size_t GetGlyphMetricsLoaded() const;
		GlyphMetrics* GetGlyphMetrics(int a_Codepoint);

		size_t GetGlyphBitmapsLoaded() const;
		GlyphBitmap* GetGlyphBitmap(int a_Codepoint);

		size_t GetOutlinesLoaded() const;
		CurvePath* CreateOutline(int a_Codepoint);

		bool TryGetKerningAdjustment(glm::vec2& a_Adjustment, int a_CodepointCurrent, int a_CodepointNext);

	private:

		IGlyphProvider& m_Provider;
		FontMetrics m_Metrics;

		std::map<int, GlyphMetrics*> m_GlyphMetrics;
		std::map<int, GlyphBitmap*> m_GlyphBitmaps;
		std::map<int, CurvePath*> m_GlyphOutlines;
	
	}; // class Face

}; // namespace ExLibris