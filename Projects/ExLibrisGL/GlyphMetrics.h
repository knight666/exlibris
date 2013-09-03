#ifndef _GLYPHMETRICS_H_
#define _GLYPHMETRICS_H_

#include <glm/glm.hpp>

namespace ExLibris
{

	struct BoundingBox
	{
		glm::vec2 minimum;
		glm::vec2 maximum;
	};

	struct GlyphMetrics
	{
		unsigned int codepoint;
		glm::vec2 offset;
		float advance;
		BoundingBox bounding_box;
	};

}; // namespace ExLibris

#endif