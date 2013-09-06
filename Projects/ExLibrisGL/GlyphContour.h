#ifndef _GLYPHCONTOUR_H_
#define _GLYPHCONTOUR_H_

#include <glm/core/type_vec2.hpp>
#include <vector>

namespace ExLibris
{

	struct GlyphContour
	{
		std::vector<glm::vec2> points;
	};

}; // namespace ExLibris

#endif