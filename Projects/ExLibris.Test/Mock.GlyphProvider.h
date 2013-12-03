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
	
		MockGlyphProvider(Library* a_Library, const std::string& a_FamilyName = "MockFamily")
			: IGlyphProvider(a_Library)
			, use_font_metrics(false)
			, use_glyph_metrics(false)
			, use_glyph_bitmaps(false)
			, use_glyph_outlines(false)
			, use_kerning_pairs(false)
		{
			if (m_Library != nullptr)
			{
				m_Family = m_Library->CreateFamily(a_FamilyName);
			}
		}

		void SetWeight(Weight a_Weight)
		{
			m_Weight = a_Weight;
		}

		void SetStyle(Style a_Style)
		{
			m_Style = a_Style;
		}

		bool HasKerning() const
		{
			return true;
		}

		bool IsScalable() const
		{
			return true;
		}

		bool IsSizeAvailable(float a_Size) const
		{
			return (size_blacklist.find(a_Size) == size_blacklist.end());
		}

		unsigned int GetIndexForCodepoint(int a_CodepointUtf32)
		{
			if (codepoint_blacklist.find(a_CodepointUtf32) != codepoint_blacklist.end())
			{
				return EXL_INVALID_INDEX;
			}
			else
			{
				return (unsigned int)a_CodepointUtf32;
			}
		}

		GlyphMetrics* CreateMetrics(float a_Size, unsigned int a_Index)
		{
			if (EXL_IS_INDEX_INVALID(a_Index) || !IsSizeAvailable(a_Size))
			{
				return nullptr;
			}

			if (use_glyph_metrics)
			{
				std::map<unsigned int, GlyphMetrics>::iterator found = glyph_metrics.find(a_Index);
				if (found != glyph_metrics.end())
				{
					return &found->second;
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				GlyphMetrics* metrics = new GlyphMetrics;

				metrics->offset = glm::vec2(4.0f, 8.0f);
				metrics->advance = 12.0f;
				metrics->bounding_box = BoundingBox(
					glm::vec2(2.0f, 2.0f),
					glm::vec2(10.0f, 10.0f)
				);

				return metrics;
			}
		}

		GlyphBitmap* CreateBitmap(float a_Size, unsigned int a_Index)
		{
			if (EXL_IS_INDEX_INVALID(a_Index) || !IsSizeAvailable(a_Size))
			{
				return nullptr;
			}

			if (use_glyph_bitmaps)
			{
				std::map<unsigned int, GlyphBitmap>::iterator found = glyph_bitmaps.find(a_Index);
				if (found != glyph_bitmaps.end())
				{
					return &found->second;
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				GlyphBitmap* bitmap = new GlyphBitmap;

				bitmap->width = 8;
				bitmap->height = 8;

				return bitmap;
			}
		}

		CurvePath* CreateOutline(float a_Size, unsigned int a_Index)
		{
			if (EXL_IS_INDEX_INVALID(a_Index) || !IsSizeAvailable(a_Size))
			{
				return nullptr;
			}

			if (use_glyph_outlines)
			{
				std::map<unsigned int, CurvePath>::iterator found = glyph_outlines.find(a_Index);
				if (found != glyph_outlines.end())
				{
					return &found->second;
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				CurvePath* outline = new CurvePath;

				outline->Move(glm::vec2(2.0f, 2.0f));
				outline->LineTo(glm::vec2(10.0f, 2.0f));
				outline->LineTo(glm::vec2(10.0f, 10.0f));
				outline->LineTo(glm::vec2(2.0f, 10.0f));
				outline->LineTo(glm::vec2(2.0f, 2.0f));

				return outline;
			}
		}

		bool TryGetKerningAdjustment(glm::vec2& a_Adjustment, float a_Size, unsigned int a_IndexCurrent, unsigned int a_IndexNext)
		{
			if (EXL_IS_INDEX_INVALID(a_IndexCurrent) || EXL_IS_INDEX_INVALID(a_IndexNext) || !IsSizeAvailable(a_Size))
			{
				return nullptr;
			}

			if (use_kerning_pairs)
			{
				for (std::vector<KerningPair>::iterator pair_it = kerning_pairs.begin(); pair_it != kerning_pairs.end(); ++pair_it)
				{
					KerningPair& pair = *pair_it;

					if (pair.index_left == a_IndexCurrent && pair.index_right == a_IndexNext)
					{
						a_Adjustment = pair.adjustment;

						return true;
					}
				}

				return false;
			}
			else
			{
				a_Adjustment.x = -1.5f;
				a_Adjustment.y = 5.6f;

				return true;
			}
		}

		Face* CreateFace(float a_Size)
		{
			if (size_blacklist.find(a_Size) != size_blacklist.end())
			{
				return nullptr;
			}

			if (use_font_metrics)
			{
				return new Face(*this, font_metrics);
			}
			else
			{
				FontMetrics metrics;
				metrics.family = m_Family;
				metrics.weight = m_Weight;
				metrics.style = m_Style;
				metrics.size = a_Size;
				metrics.line_height = a_Size * 1.5f;
				metrics.ascent = a_Size / 2.0f;
				metrics.descent = -(a_Size / 3.0f);

				return new Face(*this, metrics);
			}
		}

	public:

		std::set<int> codepoint_blacklist;
		std::set<float> size_blacklist;

		bool use_font_metrics;
		FontMetrics font_metrics;

		bool use_glyph_metrics;
		std::map<unsigned int, GlyphMetrics> glyph_metrics;

		bool use_glyph_bitmaps;
		std::map<unsigned int, GlyphBitmap> glyph_bitmaps;

		bool use_glyph_outlines;
		std::map<unsigned int, CurvePath> glyph_outlines;

		struct KerningPair
		{
			unsigned int index_left;
			unsigned int index_right;
			glm::vec2 adjustment;
		};
		bool use_kerning_pairs;
		std::vector<KerningPair> kerning_pairs;
	
	}; // class MockGlyphProvider

}; // namespace ExLibris