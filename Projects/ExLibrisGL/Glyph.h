#ifndef _GLYPH_H_
#define _GLYPH_H_

#include "GlyphBitmap.h"
#include "GlyphMetrics.h"
#include "GlyphOutline.h"

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
			}
			if (bitmap != nullptr)
			{
				delete bitmap;
			}
			if (outline != nullptr)
			{
				delete outline;
			}
		}

	public:

		unsigned int index;
		GlyphMetrics* metrics;
		GlyphBitmap* bitmap;
		GlyphOutline* outline;
		std::map<unsigned int, glm::vec2> kernings;
	
	}; // struct Glyph

}; // namespace ExLibris

#endif