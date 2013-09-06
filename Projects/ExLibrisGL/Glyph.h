#ifndef _GLYPH_H_
#define _GLYPH_H_

#include "GlyphBitmap.h"
#include "GlyphMetrics.h"

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
	
	}; // struct Glyph

}; // namespace ExLibris

#endif