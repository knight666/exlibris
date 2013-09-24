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
		glm::vec2 direction = glm::normalize(end - start);

		float thickness_half = a_Thickness / 2.0f;
		glm::vec2 offset_upper(direction.y * thickness_half, -direction.x * thickness_half);
		glm::vec2 offset_lower(-direction.y * thickness_half, direction.x * thickness_half);

		Quad quad;

		// counter-clockwise rotation

		quad.ur = end + offset_upper;
		quad.ul = start + offset_upper;

		quad.ll = start + offset_lower;
		quad.lr = end + offset_lower;

		return quad;
	}

	Line::CollisionResult Line::Collides(const Line& a_Other) const
	{
		CollisionResult result;
		result.collides = false;
		result.time = 0.0f;

		glm::vec2 delta_a = end - start;
		glm::vec2 delta_b = a_Other.end - a_Other.start;

		float b_dot_d_perp = (delta_a.x * delta_b.y) - (delta_a.y * delta_b.x);
		if (b_dot_d_perp == 0.0f)
		{
			return result;
		}

		result.collides = true;

		glm::vec2 local = a_Other.start - start;
		result.time = ((local.x * delta_b.y) - (local.y * delta_b.x)) / b_dot_d_perp;
		result.position = start + (delta_a * result.time);

		return result;
	}

}; // namespace ExLibris