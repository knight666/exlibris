#ifndef _LINESHAPE_H_
#define _LINESHAPE_H_

#include "MeshBuilder.h"
#include "Polygon.h"
#include "TriangleList.h"

namespace ExLibris
{

	class LineShape
	{

	public:
	
		LineShape();
		~LineShape();

		size_t GetPolygonCount() const;

		void Clear();
		void AddPolygon(const Polygon& a_Polygon);

		TriangleList* Triangulate(float a_Thickness) const;

		MeshBuilder* BuildMesh(float a_Thickness);

	private:

		void _TriangulatePolygon(MeshBuilder* a_Target, const Polygon& a_Polygon, float a_Thickness);

	private:

		enum ShapeType
		{
			eShapeType_Triangle,
			eShapeType_Quad
		};

		std::vector<Polygon> m_Polygons;

	}; // class LineShape

}; // namespace ExLibris

#endif