#ifndef _GLYPH_H_
#define _GLYPH_H_

#include "GlyphBitmap.h"
#include "GlyphMetrics.h"
#include "GlyphOutline.h"
#include "TriangleList.h"

namespace ExLibris
{

	struct Glyph
	{

		Glyph()
			: index(0)
			, metrics(nullptr)
			, bitmap(nullptr)
			, outline(nullptr)
			, mesh(nullptr)
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

			if (mesh != nullptr)
			{
				delete mesh;
				mesh = nullptr;
			}
		}

	public:

		unsigned int index;
		GlyphMetrics* metrics;
		GlyphBitmap* bitmap;
		GlyphOutline* outline;
		TriangleList* mesh;
		std::map<unsigned int, glm::vec2> kernings;
	
	}; // struct Glyph

}; // namespace ExLibris

#endif