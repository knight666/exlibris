#include "ExLibrisGL.PCH.h"

#include "Polygon.h"

namespace ExLibris
{

	Polygon& Polygon::operator += (glm::vec2& a_Position)
	{
		positions.push_back(a_Position);

		return *this;
	}

	bool Polygon::IsPositionInside(const glm::vec2& a_Position) const
	{
		if (positions.size() < 3)
		{
			return false;
		}

		bool inside = false;

		std::vector<glm::vec2>::const_iterator start_it = positions.begin();
		std::vector<glm::vec2>::const_iterator end_it = positions.end() - 1;

		while (start_it != positions.end())
		{
			const glm::vec2& start = *start_it;
			const glm::vec2& end = *end_it;

			if ((start.y > a_Position.y) != (end.y > a_Position.y))
			{
				bool raycast = (a_Position.x < ((end.x - start.x) * (a_Position.y - start.y) / (end.y - start.y) + start.x));
				if (raycast)
				{
					inside = !inside;
				}
			}

			end_it = start_it++;
		}

		return inside;
	}

	bool Polygon::Intersects(const Polygon& a_Other) const
	{
		if (positions.size() < 3 || a_Other.positions.size() < 3)
		{
			return false;
		}

		for (std::vector<glm::vec2>::const_iterator position_it = a_Other.positions.begin(); position_it != a_Other.positions.end(); ++position_it)
		{
			if (IsPositionInside(*position_it))
			{
				return true;
			}
		}

		return false;
	}

}; // namespace ExLibris