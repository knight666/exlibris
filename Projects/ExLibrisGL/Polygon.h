#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <glm/glm.hpp>
#include <vector>

namespace ExLibris
{

	class Polygon
	{

	public:

		bool IsPositionInside(const glm::vec2& a_Position) const;

		bool Intersects(const Polygon& a_Other) const;
	
	public:

		std::vector<glm::vec2> positions;
	
	}; // class Polygon

}; // namespace ExLibris

#endif