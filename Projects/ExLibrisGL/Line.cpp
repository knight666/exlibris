#include "ExLibrisGL.PCH.h"

#include "Line.h"

namespace ExLibris
{

	Line::Line()
	{
	}
	
	Line::Line(const glm::vec2& a_Start, const glm::vec2& a_End)
		: start(a_Start)
		, end(a_End)
	{
	}

}; // namespace ExLibris