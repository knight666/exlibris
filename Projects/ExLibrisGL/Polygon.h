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

		std::vector<Shape> m_Shapes;
	
	}; // class Polygon

}; // namespace ExLibris

#endif