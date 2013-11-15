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

#include <glm/glm.hpp>

#include "FontMetrics.h"

namespace ExLibris
{
	class CurvePath;
	class Family;
	class Face;
	struct GlyphBitmap;
	struct GlyphMetrics;
	class Library;
}

namespace ExLibris
{

	class IGlyphProvider
	{
	
	public:
	
		IGlyphProvider(Library* a_Library)
			: m_Library(a_Library)
			, m_Family(nullptr)
			, m_Weight(eWeight_Normal)
			, m_Style(eStyle_None)
		{
		}

		virtual ~IGlyphProvider()
		{
		}

	public:

		Library* GetLibrary() const
		{
			return m_Library;
		}

		Family* GetFamily() const
		{
			return m_Family;
		}

		Weight GetWeight() const
		{
			return m_Weight;
		}

		Style GetStyle() const
		{
			return m_Style;
		}

		int GetMatchScore(float a_Size, Weight a_Weight, Style a_Style) const
		{
			if (!IsSizeAvailable(a_Size))
			{
				return -1;
			}

			int score = 0;

			if (GetStyle() == a_Style)
			{
				score += 10;
			}
			if (GetWeight() == a_Weight)
			{
				score += 100;
			}

			return score;
		}

	public:

		virtual bool HasKerning() const = 0;
		virtual bool IsScalable() const = 0;
		virtual bool IsSizeAvailable(float a_Size) const = 0;
	
		virtual GlyphMetrics* CreateMetrics(float a_Size, int a_Codepoint) = 0;
		virtual GlyphBitmap* CreateBitmap(float a_Size, int a_Codepoint) = 0;
		virtual CurvePath* CreateOutline(float a_Size, int a_Codepoint) = 0;

		virtual bool TryGetKerningAdjustment(glm::vec2& a_Adjustment, float a_Size, int a_CodepointCurrent, int a_CodepointNext) = 0;

		virtual Face* CreateFace(float a_Size) = 0;

	protected:

		Library* m_Library;
		Family* m_Family;
		Weight m_Weight;
		Style m_Style;
	
	}; // class IGlyphProvider

}; // namespace ExLibris