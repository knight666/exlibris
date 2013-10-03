#ifndef _LINESHAPE_H_
#define _LINESHAPE_H_

#include <poly2tri/poly2tri.h>

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

		MeshBuilder* BuildFilledMesh() const;
		MeshBuilder* BuildOutlineMesh(const LineMeshOptions& a_Options) const;

	private:

		void _CheckPolylineOverlap(std::vector<p2t::Point*>& a_Polyline) const;
		void _TriangulateOutlineFast(MeshBuilder* a_Target, const Polygon& a_Polygon, float a_Thickness) const;
		void _TriangulateOutlineGapless(MeshBuilder* a_Target, const Polygon& a_Polygon, float a_Thickness) const;

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