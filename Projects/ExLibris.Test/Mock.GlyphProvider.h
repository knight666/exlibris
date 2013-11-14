#pragma once

#include <set>

#include <CurvePath.h>
#include <Face.h>
#include <GlyphBitmap.h>
#include <GlyphMetrics.h>
#include <IGlyphProvider.h>
#include <Library.h>

namespace ExLibris
{

	class MockGlyphProvider
		: public IGlyphProvider
	{
	
	public:
	
		MockGlyphProvider(Library* a_Library)
			: IGlyphProvider(a_Library)
		{
		}

		bool HasKerning() const
		{
			return true;
		}
	
		GlyphMetrics* CreateMetrics(float a_Size, int a_Codepoint)
		{
			if (
				size_blacklist.find(a_Size) != size_blacklist.end() ||
				codepoint_blacklist.find(a_Codepoint) != codepoint_blacklist.end()
			)
			{
				return nullptr;
			}

			GlyphMetrics* metrics = new GlyphMetrics;

			metrics->offset = glm::vec2(4.0f, 8.0f);
			metrics->advance = 12.0f;
			metrics->bounding_box = BoundingBox(
				glm::vec2(2.0f, 2.0f),
				glm::vec2(10.0f, 10.0f)
			);

			return metrics;
		}

		GlyphBitmap* CreateBitmap(float a_Size, int a_Codepoint)
		{
			if (
				size_blacklist.find(a_Size) != size_blacklist.end() ||
				codepoint_blacklist.find(a_Codepoint) != codepoint_blacklist.end()
			)
			{
				return nullptr;
			}

			GlyphBitmap* bitmap = new GlyphBitmap;
			
			bitmap->width = 8;
			bitmap->height = 8;

			return bitmap;
		}

		CurvePath* CreateOutline(float a_Size, int a_Codepoint)
		{
			if (
				size_blacklist.find(a_Size) != size_blacklist.end() ||
				codepoint_blacklist.find(a_Codepoint) != codepoint_blacklist.end()
			)
			{
				return nullptr;
			}

			CurvePath* outline = new CurvePath;

			outline->Move(glm::vec2(2.0f, 2.0f));
			outline->LineTo(glm::vec2(10.0f, 2.0f));
			outline->LineTo(glm::vec2(10.0f, 10.0f));
			outline->LineTo(glm::vec2(2.0f, 10.0f));
			outline->LineTo(glm::vec2(2.0f, 2.0f));

			return outline;
		}

		bool TryGetKerningAdjustment(glm::vec2& a_Adjustment, float a_Size, int a_CodepointCurrent, int a_CodepointNext)
		{
			if (
				size_blacklist.find(a_Size) != size_blacklist.end() ||
				codepoint_blacklist.find(a_CodepointCurrent) != codepoint_blacklist.end() ||
				codepoint_blacklist.find(a_CodepointNext) != codepoint_blacklist.end()
			)
			{
				return nullptr;
			}

			a_Adjustment.x = -1.5f;
			a_Adjustment.y = 5.6f;

			return true;
		}

		Face* CreateFace(float a_Size)
		{
			if (size_blacklist.find(a_Size) != size_blacklist.end())
			{
				return nullptr;
			}

			FontMetrics metrics;
			metrics.size = a_Size;
			metrics.line_height = a_Size * 1.5f;
			metrics.ascent = a_Size / 2.0f;
			metrics.descent = -(a_Size / 3.0f);

			return new Face(*this, metrics);
		}

	public:

		std::set<int> codepoint_blacklist;
		std::set<float> size_blacklist;
	
	}; // class MockGlyphProvider

}; // namespace ExLibris