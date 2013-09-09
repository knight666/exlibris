#ifndef _GLYPH_H_
#define _GLYPH_H_

#include "GlyphBitmap.h"
#include "GlyphMetrics.h"
#include "GlyphOutline.h"

namespace ExLibris
{

	struct Glyph
	{

	public:

		Glyph();

	public:

		unsigned int index;
		GlyphMetrics* metrics;
		GlyphBitmap* bitmap;
		GlyphOutline* outline;
		std::map<unsigned int, glm::vec2> kernings;
	
	}; // struct Glyph

}; // namespace ExLibris

#endif