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

#include "Face.h"

namespace ExLibris
{

	Face::Face(IGlyphProvider& a_Provider, const FontMetrics& a_Metrics)
		: m_Provider(a_Provider)
		, m_Metrics(a_Metrics)
	{
	}
	
	Face::~Face()
	{
	}

	Family* Face::GetFamily() const
	{
		return m_Metrics.family;
	}

	float Face::GetSize() const
	{
		return m_Metrics.size;
	}

	float Face::GetLineHeight() const
	{
		return m_Metrics.line_height;
	}

	float Face::GetAscent() const
	{
		return m_Metrics.ascent;
	}

	float Face::GetDescent() const
	{
		return m_Metrics.descent;
	}

	GlyphMetrics* Face::GetGlyphMetrics(int a_Codepoint)
	{
		std::map<int, GlyphMetrics*>::iterator found = m_GlyphMetrics.find(a_Codepoint);
		if (found != m_GlyphMetrics.end())
		{
			return found->second;
		}
		else
		{
			GlyphMetrics* metrics = m_Provider.CreateMetrics(m_Metrics.size, a_Codepoint);
			
			m_GlyphMetrics.insert(std::make_pair(a_Codepoint, metrics));

			return metrics;
		}
	}

	GlyphBitmap* Face::GetGlyphBitmap(int a_Codepoint)
	{
		std::map<int, GlyphBitmap*>::iterator found = m_GlyphBitmaps.find(a_Codepoint);
		if (found != m_GlyphBitmaps.end())
		{
			return found->second;
		}
		else
		{
			GlyphBitmap* bitmap = m_Provider.CreateBitmap(m_Metrics.size, a_Codepoint);

			m_GlyphBitmaps.insert(std::make_pair(a_Codepoint, bitmap));

			return bitmap;
		}
	}

	CurvePath* Face::GetGlyphOutline(int a_Codepoint)
	{
		std::map<int, CurvePath*>::iterator found = m_GlyphOutlines.find(a_Codepoint);
		if (found != m_GlyphOutlines.end())
		{
			return found->second;
		}
		else
		{
			CurvePath* outline = m_Provider.CreateOutline(m_Metrics.size, a_Codepoint);

			m_GlyphOutlines.insert(std::make_pair(a_Codepoint, outline));

			return outline;
		}
	}

	bool Face::TryGetKerningAdjustment(glm::vec2& a_Adjustment, int a_CodepointCurrent, int a_CodepointNext)
	{
		return m_Provider.TryGetKerningAdjustment(a_Adjustment, m_Metrics.size, a_CodepointCurrent, a_CodepointNext);
	}

}; // namespace ExLibris