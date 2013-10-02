#ifndef _GLYPH_H_
#define _GLYPH_H_

#include "CurvePath.h"
#include "GlyphBitmap.h"
#include "GlyphMetrics.h"

namespace ExLibris
{

	struct Glyph
	{

		Glyph()
			: index(0)
			, metrics(nullptr)
			, bitmap(nullptr)
			, outline(nullptr)
		{
		}

		~Glyph()
		{
			if (metrics != nullptr)
			{
				delete metrics;
				metrics = nullptr;
			}
			
			if (bitmap != nullptr)
			{
				delete bitmap;
				bitmap = nullptr;
			}

			if (outline != nullptr)
			{
				delete outline;
				outline = nullptr;
			}
		}

	public:

		unsigned int index;
		GlyphMetrics* metrics;
		GlyphBitmap* bitmap;
		CurvePath* outline;
		std::map<unsigned int, glm::vec2> kernings;
	
	}; // struct Glyph

}; // namespace ExLibris

#endif