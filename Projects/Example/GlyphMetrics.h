#ifndef _GLYPHMETRICS_H_
#define _GLYPHMETRICS_H_

#include <glm/glm.hpp>

namespace ExLibris
{

	struct GlyphMetrics
	{
		unsigned int codepoint;
		glm::vec2 offset;
		float advance;
	};

}; // namespace ExLibris

#endif