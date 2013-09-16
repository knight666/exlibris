#include "ExLibrisGL.PCH.h"

#include "Triangle.h"

namespace ExLibris
{

	Triangle::Triangle()
	{
	}

	Triangle::Triangle(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C)
		: a(a_A)
		, b(a_B)
		, c(a_C)
	{
	}

	bool Triangle::IsConvex() const
	{
		return (((a.y - b.y) * (c.x - b.x)) + ((b.x - a.x) * (c.y - b.y))) >= 0.0f;
	}

	bool Triangle::IsPositionInside(const glm::vec2& a_Position) const
	{
		glm::vec2 v0 = c - a;
		glm::vec2 v1 = b - a;
		glm::vec2 v2 = a_Position - a;

		float dot00 = glm::dot(v0, v0);
		float dot01 = glm::dot(v0, v1);
		float dot02 = glm::dot(v0, v2);
		float dot11 = glm::dot(v1, v1);
		float dot12 = glm::dot(v1, v2);

		float denominator = dot00 * dot11 - dot01 * dot01;

		float u = (dot11 * dot02 - dot01 * dot12) / denominator;
		if (u < 0.0f)
		{
			return false;
		}

		float v = (dot00 * dot12 - dot01 * dot02) / denominator;
		if (v < 0.0f)
		{
			return false;
		}

		return (u + v) < 1.0f;
	}

}; // namespace ExLibris