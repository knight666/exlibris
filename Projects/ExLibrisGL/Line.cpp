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
		Quad quad;

		if (glm::distance(end, start) < 1e-6f)
		{
			// counter-clockwise rotation

			quad.ur = end;
			quad.ul = start;

			quad.ll = start;
			quad.lr = end;
		}
		else
		{
			glm::vec2 direction = glm::normalize(end - start);

			float thickness_half = a_Thickness / 2.0f;
			glm::vec2 offset_upper(direction.y * thickness_half, -direction.x * thickness_half);
			glm::vec2 offset_lower(-direction.y * thickness_half, direction.x * thickness_half);

			// counter-clockwise rotation

			quad.ur = end + offset_upper;
			quad.ul = start + offset_upper;

			quad.ll = start + offset_lower;
			quad.lr = end + offset_lower;
		}

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

	float Line::GetPerpendicularDistanceToPosition(const glm::vec2& a_Position) const
	{
		glm::vec2 line_difference = end - start;
		glm::vec2 start_to_point = a_Position - start;

		float line_length_squared = (line_difference.x * line_difference.x) + (line_difference.y * line_difference.y);
		if (line_length_squared == 0.0f)
		{
			return glm::length(start_to_point);
		}

		float time = glm::dot(start_to_point, line_difference) / line_length_squared;

		if (time < 0.0f)
		{
			return glm::length(start_to_point);
		}
		else if (time > 1.0f)
		{
			return glm::distance(a_Position, end);
		}
		else
		{
			glm::vec2 projection = start + (line_difference * time);
			return glm::distance(a_Position, projection);
		}
	}

	float Line::GetCrossProduct(const glm::vec2& a_Position) const
	{
		glm::vec2 projected_line = end - start;
		glm::vec2 projected_position = a_Position - start;

		return ((projected_line.x * projected_position.y) - (projected_line.y * projected_position.x));
	}

}; // namespace ExLibris