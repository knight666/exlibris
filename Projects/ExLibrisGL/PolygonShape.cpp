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
		m_Shapes.clear();
	}

	void PolygonShape::AddShape(const Shape& a_Shape)
	{
		m_Shapes.push_back(a_Shape);
	}

	std::vector<p2t::Point*> ConvertShapeToPolyline(const Shape& a_Shape)
	{
		std::vector<p2t::Point*> polyline;

		for (std::vector<glm::vec2>::const_iterator position_it = a_Shape.positions.begin(); position_it != a_Shape.positions.end(); ++position_it)
		{
			const glm::vec2& position = *position_it;

			polyline.push_back(new p2t::Point((double)position.x, (double)position.y));
		}

		// check if last and first position overlap

		p2t::Point* position_first = polyline.front();
		p2t::Point* position_last = polyline.back();

		double delta = 1e-6;
		if (abs(position_first->x - position_last->x) < delta && abs(position_first->y - position_last->y) < delta)
		{
			delete position_last;
			polyline.pop_back();
		}

		return polyline;
	}

	// if an object is returned instead of a pointer, the destructor is called on return
	// compiler bug?

	TriangleList* PolygonShape::Triangulate() const
	{
		TriangleList* triangles = new TriangleList;

		if (m_Shapes.size() == 0)
		{
			return triangles;
		}

		std::vector<p2t::Point*> base_polyline = ConvertShapeToPolyline(*m_Shapes.begin());
		p2t::CDT* cdt = new p2t::CDT(base_polyline);

		if (m_Shapes.size() > 1)
		{
			for (std::vector<Shape>::const_iterator shape_it = m_Shapes.begin() + 1; shape_it != m_Shapes.end(); ++shape_it)
			{
				const Shape& shape = *shape_it;

				std::vector<p2t::Point*> hole = ConvertShapeToPolyline(shape);
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

	bool PolygonShape::_IsConvex(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C) const
	{
		return (((a_A.y - a_B.y) * (a_C.x - a_B.x)) + ((a_B.x - a_A.x) * (a_C.y - a_B.y))) >= 0;
	}

	bool PolygonShape::_IsPointInTriangle(const glm::vec2& a_TriangleA, const glm::vec2& a_TriangleB, const glm::vec2& a_TriangleC, const glm::vec2& a_Position) const
	{
		glm::vec2 v0 = a_TriangleC - a_TriangleA;
		glm::vec2 v1 = a_TriangleB - a_TriangleA;
		glm::vec2 v2 = a_Position - a_TriangleA;

		float dot00 = glm::dot(v0, v0);
		float dot01 = glm::dot(v0, v1);
		float dot02 = glm::dot(v0, v2);
		float dot11 = glm::dot(v1, v1);
		float dot12 = glm::dot(v1, v2);

		float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);

		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		if (u < 0.0f)
		{
			return false;
		}

		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
		if (v < 0.0f)
		{
			return false;
		}

		return (u + v) < 1.0f;
	}

}; // namespace ExLibris