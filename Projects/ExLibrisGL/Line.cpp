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

	Quad Line::ConstructQuad(float a_Thickness) const
	{
		glm::vec2 quad_start = start;
		glm::vec2 quad_end = end;

		if (quad_start.x > quad_end.x)
		{
			quad_end.x = start.x;
			quad_start.x = end.x;
		}

		if (quad_start.y > quad_end.y)
		{
			quad_end.y = start.y;
			quad_start.y = end.y;
		}

		glm::vec2 direction = glm::normalize(quad_end - quad_start);

		float thickness_half = a_Thickness / 2.0f;
		glm::vec2 offset_upper(direction.y * thickness_half, -direction.x * thickness_half);
		glm::vec2 offset_lower(-direction.y * thickness_half, direction.x * thickness_half);

		Quad quad;

		// counter-clockwise rotation

		quad.ur = quad_end + offset_upper;
		quad.ul = quad_start + offset_upper;

		quad.ll = quad_start + offset_lower;
		quad.lr = quad_end + offset_lower;

		return quad;
	}

}; // namespace ExLibris