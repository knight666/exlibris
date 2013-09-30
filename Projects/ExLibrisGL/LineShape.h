#ifndef _LINESHAPE_H_
#define _LINESHAPE_H_

#include "LineMeshOptions.h"
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

		MeshBuilder* BuildMesh(const LineMeshOptions& a_Options);

	private:

		void _TriangulatePolygonFast(MeshBuilder* a_Target, const Polygon& a_Polygon, float a_Thickness);
		void _TriangulatePolygonGapless(MeshBuilder* a_Target, const Polygon& a_Polygon, float a_Thickness);

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