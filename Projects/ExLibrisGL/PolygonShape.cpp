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

	struct PolyConvert
	{
		PolyConvert()
			: polygon(nullptr)
			, cdt(nullptr)
		{
		}

		~PolyConvert()
		{
			if (cdt != nullptr)
			{
				delete cdt;
				cdt = nullptr;
			}
		}

		const Polygon* polygon;
		std::vector<p2t::Point*> polyline;
		std::vector<const Polygon*> holes;
		p2t::CDT* cdt;
		std::vector<p2t::Triangle*> triangles;
	};

	// if an object is returned instead of a pointer, the destructor is called on return
	// compiler bug?

	TriangleList* PolygonShape::Triangulate() const
	{
		TriangleList* triangles = new TriangleList;

		if (m_Polygons.size() == 0)
		{
			return triangles;
		}

		std::vector<PolyConvert*> converted;
		PolyConvert* convert_current = nullptr;

		for (std::vector<Polygon>::const_iterator polygon_it = m_Polygons.begin(); polygon_it != m_Polygons.end(); ++polygon_it)
		{
			const Polygon& polygon = *polygon_it;

			bool is_base = false;

			if (convert_current != nullptr)
			{
				if (convert_current->polygon->Intersects(polygon))
				{
					convert_current->holes.push_back(&polygon);
				}
				else
				{
					is_base = true;
				}
			}
			else
			{
				is_base = true;
			}

			if (is_base)
			{
				convert_current = new PolyConvert;
				convert_current->polygon = &polygon;

				for (std::vector<glm::vec2>::const_iterator position_it = polygon.positions.begin(); position_it != polygon.positions.end(); ++position_it)
				{
					const glm::vec2& position = *position_it;

					convert_current->polyline.push_back(
						new p2t::Point(
							(double)position.x,
							(double)position.y
						)
					);
				}

				converted.push_back(convert_current);
			}
		}

		for (std::vector<PolyConvert*>::iterator convert_it = converted.begin(); convert_it != converted.end(); ++convert_it)
		{
			PolyConvert* convert = *convert_it;

			CheckPolylineOverlap(convert->polyline);
			convert->cdt = new p2t::CDT(convert->polyline);

			if (convert->holes.size() > 0)
			{
				for (std::vector<const Polygon*>::iterator hole_it = convert->holes.begin(); hole_it != convert->holes.end(); ++hole_it)
				{
					const Polygon* polygon = *hole_it;

					std::vector<p2t::Point*> hole_polyline;
					for (std::vector<glm::vec2>::const_iterator position_it = polygon->positions.begin(); position_it != polygon->positions.end(); ++position_it)
					{
						const glm::vec2& position = *position_it;

						hole_polyline.push_back(
							new p2t::Point(
								(double)position.x,
								(double)position.y
							)
						);
					}

					CheckPolylineOverlap(hole_polyline);

					convert->cdt->AddHole(hole_polyline);
				}
			}

			convert->cdt->Triangulate();

			convert->triangles = convert->cdt->GetTriangles();
			triangles->vertex_count += convert->triangles.size() * 3;
		}

		triangles->positions = new glm::vec2[triangles->vertex_count];
		glm::vec2* dst_position_data = triangles->positions;

		for (std::vector<PolyConvert*>::iterator convert_it = converted.begin(); convert_it != converted.end(); ++convert_it)
		{
			PolyConvert* convert = *convert_it;

			for (std::vector<p2t::Triangle*>::iterator triangle_it = convert->triangles.begin(); triangle_it != convert->triangles.end(); ++triangle_it)
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

			delete convert;
		}

		converted.clear();
		
		return triangles;
	}

}; // namespace ExLibris