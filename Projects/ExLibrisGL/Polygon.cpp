#include "ExLibrisGL.PCH.h"

#include "Polygon.h"

namespace ExLibris
{

	/*int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
	{
		int i, j, c = 0;
		for (i = 0, j = nvert-1; i < nvert; j = i++) {
			if ( ((verty[i]>testy) != (verty[j]>testy)) &&
				(
					testx < 
						(vertx[j] - vertx[i]) * 
						(testy - verty[i]) / 
						(verty[j] - verty[i]) + 
						vertx[i]
				) )
				c = !c;
		}
		return c;
	}*/

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

			bool vertical = ((start.y > a_Position.y) != (end.y > a_Position.y));
			bool horizontal = (a_Position.x < ((end.x - start.x) * (a_Position.y - start.y) / (end.y - start.y) + start.x));

			if (vertical && horizontal)
			{
				inside = !inside;
			}

			end_it = start_it++;
		}

		return inside;
	}

}; // namespace ExLibris