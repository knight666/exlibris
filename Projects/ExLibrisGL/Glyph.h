#ifndef _GLYPH_H_
#define _GLYPH_H_

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
	
	}; // struct Glyph

}; // namespace ExLibris

#endif