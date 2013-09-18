#include "ExLibrisGL.PCH.h"

#include "PolygonShape.h"

#include <poly2tri/poly2tri.h>

#include "Triangle.h"

namespace ExLibris
{

	PolygonShape::PolygonShape()
	{
	}
	
	PolygonShape::~PolygonShape()
	{
		m_Polygons.clear();
	}

	void PolygonShape::AddShape(const Polygon& a_Shape)
	{
		m_Polygons.push_back(a_Shape);
	}

	void CheckPolylineOverlap(std::vector<p2t::Point*>& a_Polyline)
	{
		// check if last and first position overlap

		p2t::Point* position_first = a_Polyline.front();
		p2t::Point* position_last = a_Polyline.back();

		double delta = 1e-6;
		if (abs(position_first->x - position_last->x) < delta && abs(position_first->y - position_last->y) < delta)
		{
			delete position_last;
			a_Polyline.pop_back();
		}
	}

	std::vector<p2t::Point*> ConvertPolygonToPolyline(const Polygon& a_Shape)
	{
		std::vector<p2t::Point*> polyline;

		for (std::vector<glm::vec2>::const_iterator position_it = a_Shape.positions.begin(); position_it != a_Shape.positions.end(); ++position_it)
		{
			const glm::vec2& position = *position_it;

			polyline.push_back(new p2t::Point((double)position.x, (double)position.y));
		}

		CheckPolylineOverlap(polyline);

		return polyline;
	}

	// if an object is returned instead of a pointer, the destructor is called on return
	// compiler bug?

	TriangleList* PolygonShape::Triangulate() const
	{
		TriangleList* triangles = new TriangleList;

		if (m_Polygons.size() == 0)
		{
			return triangles;
		}

		std::vector<p2t::Point*> base_polyline = ConvertPolygonToPolyline(*m_Polygons.begin());
		p2t::CDT* cdt = new p2t::CDT(base_polyline);

		if (m_Polygons.size() > 1)
		{
			for (std::vector<Polygon>::const_iterator shape_it = m_Polygons.begin() + 1; shape_it != m_Polygons.end(); ++shape_it)
			{
				const Polygon& shape = *shape_it;

				std::vector<p2t::Point*> hole = ConvertPolygonToPolyline(shape);
				cdt->AddHole(hole);
			}
		}

		cdt->Triangulate();

		std::vector<p2t::Triangle*> cdt_triangles = cdt->GetTriangles();

		triangles->vertex_count = cdt_triangles.size() * 3;

		triangles->positions = new glm::vec2[triangles->vertex_count];
		glm::vec2* dst_position_data = triangles->positions;

		for (std::vector<p2t::Triangle*>::iterator triangle_it = cdt_triangles.begin(); triangle_it != cdt_triangles.end(); ++triangle_it)
		{
			p2t::Triangle* triangle = *triangle_it;

			p2t::Point* point_a = triangle->GetPoint(0);
			dst_position_data[0].x = (float)point_a->x;
			dst_position_data[0].y = (float)point_a->y;

			p2t::Point* point_b = triangle->GetPoint(1);
			dst_position_data[1].x = (float)point_b->x;
			dst_position_data[1].y = (float)point_b->y;

			p2t::Point* point_c = triangle->GetPoint(2);
			dst_position_data[2].x = (float)point_c->x;
			dst_position_data[2].y = (float)point_c->y;

			dst_position_data += 3;
		}

		delete cdt;
		
		return triangles;
	}

}; // namespace ExLibris