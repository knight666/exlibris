#ifndef _GLYPHOUTLINE_H_
#define _GLYPHOUTLINE_H_

#include "GlyphContour.h"

namespace ExLibris
{

	struct GlyphOutline
	{
		~GlyphOutline()
		{
			for (std::vector<GlyphContour*>::iterator contour_it = contours.begin(); contour_it != contours.end(); ++contour_it)
			{
				delete *contour_it;
			}
			contours.clear();
		}
	
	public:

		std::vector<GlyphContour*> contours;
	};

}; // namespace ExLibris

#endif