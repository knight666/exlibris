#ifndef _GLYPHOUTLINE_H_
#define _GLYPHOUTLINE_H_

#include "Polygon.h"

namespace ExLibris
{

	struct GlyphOutline
	{
		std::vector<Polygon> contours;
	};

}; // namespace ExLibris

#endif