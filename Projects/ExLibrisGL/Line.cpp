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
		glm::vec2 world_vertical(1.0f, -1.0f);

		glm::vec2 direction = glm::normalize(end - start);

		glm::vec2 quad_start = start;
		glm::vec2 quad_end = end;

		float horizontal = glm::dot(direction, world_vertical);
		if (horizontal < 0.0f)
		{
			direction *= -1.0f;

			quad_start.x = end.x;
			quad_end.x = start.x;
		}

		float thickness_half = a_Thickness / 2.0f;
		glm::vec2 offset_upper(direction.y * thickness_half, -direction.x * thickness_half);
		glm::vec2 offset_lower(-direction.y * thickness_half, direction.x * thickness_half);

		Quad quad(
			quad_start + offset_upper,
			quad_end + offset_upper,
			quad_start + offset_lower,
			quad_end + offset_lower
		);

		return quad;
	}

}; // namespace ExLibris