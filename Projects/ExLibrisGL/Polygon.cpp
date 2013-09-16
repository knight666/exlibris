#include "ExLibrisGL.PCH.h"

#include "Polygon.h"

namespace ExLibris
{

	Polygon::Polygon()
	{
	}
	
	Polygon::~Polygon()
	{
		m_Shapes.clear();
	}

	void Polygon::AddShape(const Shape& a_Shape)
	{
		m_Shapes.push_back(a_Shape);
	}

	TriangleList Polygon::Triangulate() const
	{
		TriangleList triangles;

		return triangles;
	}

}; // namespace ExLibris