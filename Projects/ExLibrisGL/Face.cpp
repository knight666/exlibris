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

#include "CurvePath.h"
#include "GlyphBitmap.h"
#include "GlyphMetrics.h"

namespace ExLibris
{

	Face::Face(IGlyphProvider& a_Provider, const FontMetrics& a_Metrics)
		: m_Provider(a_Provider)
		, m_Metrics(a_Metrics)
	{
	}
	
	Face::~Face()
	{
		for (std::map<int, GlyphMetrics*>::iterator metrics_it = m_GlyphMetrics.begin(); metrics_it != m_GlyphMetrics.end(); ++metrics_it)
		{
			GlyphMetrics* metrics = metrics_it->second;
			if (metrics != nullptr)
			{
				delete metrics;
			}
		}
		m_GlyphMetrics.clear();

		for (std::map<int, GlyphBitmap*>::iterator bitmap_it = m_GlyphBitmaps.begin(); bitmap_it != m_GlyphBitmaps.end(); ++bitmap_it)
		{
			GlyphBitmap* bitmap = bitmap_it->second;
			if (bitmap != nullptr)
			{
				delete bitmap;
			}
		}
		m_GlyphBitmaps.clear();

		for (std::map<int, CurvePath*>::iterator outline_it = m_GlyphOutlines.begin(); outline_it != m_GlyphOutlines.end(); ++outline_it)
		{
			CurvePath* outline = outline_it->second;
			if (outline != nullptr)
			{
				delete outline;
			}
		}
		m_GlyphOutlines.clear();
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

	size_t Face::GetGlyphMetricsLoaded() const
	{
		return m_GlyphMetrics.size();
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

	size_t Face::GetGlyphBitmapsLoaded() const
	{
		return m_GlyphBitmaps.size();
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

	size_t Face::GetGlyphOutlinesLoaded() const
	{
		return m_GlyphOutlines.size();
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