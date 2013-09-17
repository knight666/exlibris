#ifndef _POLYGONSHAPE_H_
#define _POLYGONSHAPE_H_

#include "Polygon.h"
#include "TriangleList.h"

namespace ExLibris
{

	class PolygonShape
	{
	
	public:
	
		PolygonShape();
		~PolygonShape();
	
		void AddShape(const Polygon& a_Shape);

		TriangleList* Triangulate() const;

	private:

		bool _IsConvex(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C) const;
		bool _IsPointInTriangle(const glm::vec2& a_TriangleA, const glm::vec2& a_TriangleB, const glm::vec2& a_TriangleC, const glm::vec2& a_Position) const;

	private:

		std::vector<Polygon> m_Shapes;
	
	}; // class PolygonShape

}; // namespace ExLibris

#endif