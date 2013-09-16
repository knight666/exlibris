#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "Shape.h"
#include "TriangleList.h"

namespace ExLibris
{

	class Polygon
	{
	
	public:
	
		Polygon();
		~Polygon();
	
		void AddShape(const Shape& a_Shape);

		TriangleList Triangulate() const;

	private:

		bool _IsConvex(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C) const;
		bool _IsPointInTriangle(const glm::vec2& a_TriangleA, const glm::vec2& a_TriangleB, const glm::vec2& a_TriangleC, const glm::vec2& a_Position) const;

	private:

		std::vector<Shape> m_Shapes;
	
	}; // class Polygon

}; // namespace ExLibris

#endif