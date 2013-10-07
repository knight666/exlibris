#include "ExLibrisGL.PCH.h"

#include "LineShape.h"

#include "Line.h"

namespace ExLibris
{

	LineShape::LineShape()
	{
	}
	
	LineShape::~LineShape()
	{
	}

	size_t LineShape::GetPolygonCount() const
	{
		return m_Polygons.size();
	}

	void LineShape::Clear()
	{
		m_Polygons.clear();
	}

	void LineShape::AddPolygon(const Polygon& a_Polygon)
	{
		m_Polygons.push_back(a_Polygon);
	}

	MeshBuilder* LineShape::BuildOutlineMesh(const LineMeshOptions& a_Options) const
	{
		MeshBuilder* builder = new MeshBuilder;

		if (m_Polygons.size() == 0)
		{
			return builder;
		}

		if (a_Options.quality == LineMeshOptions::eQuality_Fast)
		{
			for (std::vector<Polygon>::const_iterator polygon_it = m_Polygons.begin(); polygon_it != m_Polygons.end(); ++polygon_it)
			{
				const Polygon& polygon = *polygon_it;

				_TriangulateOutlineFast(builder, polygon, a_Options.thickness);
			}
		}
		else if (a_Options.quality == LineMeshOptions::eQuality_Gapless)
		{
			for (std::vector<Polygon>::const_iterator polygon_it = m_Polygons.begin(); polygon_it != m_Polygons.end(); ++polygon_it)
			{
				const Polygon& polygon = *polygon_it;

				_TriangulateOutlineGapless(builder, polygon, a_Options.thickness);
			}
		}
		
		return builder;
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

	MeshBuilder* LineShape::BuildFilledMesh() const
	{
		MeshBuilder* builder = new MeshBuilder;

		if (m_Polygons.size() == 0)
		{
			return builder;
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

			_CheckPolylineOverlap(convert->polyline);
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

					_CheckPolylineOverlap(hole_polyline);

					convert->cdt->AddHole(hole_polyline);
				}
			}

			convert->cdt->Triangulate();
			convert->triangles = convert->cdt->GetTriangles();
		}

		for (std::vector<PolyConvert*>::iterator convert_it = converted.begin(); convert_it != converted.end(); ++convert_it)
		{
			PolyConvert* convert = *convert_it;

			for (std::vector<p2t::Triangle*>::iterator triangle_it = convert->triangles.begin(); triangle_it != convert->triangles.end(); ++triangle_it)
			{
				p2t::Triangle* triangle = *triangle_it;

				p2t::Point* point_a = triangle->GetPoint(0);
				p2t::Point* point_b = triangle->GetPoint(1);
				p2t::Point* point_c = triangle->GetPoint(2);

				glm::vec2 a((float)point_a->x, (float)point_a->y);
				glm::vec2 b((float)point_b->x, (float)point_b->y);
				glm::vec2 c((float)point_c->x, (float)point_c->y);

				//builder->AddTriangle(a, b, c);
				builder->AddTriangle(c, b, a);
			}

			delete convert;
		}

		converted.clear();

		return builder;
	}

	void LineShape::_CheckPolylineOverlap(std::vector<p2t::Point*>& a_Polyline) const
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

	void LineShape::_TriangulateOutlineFast(MeshBuilder* a_Target, const Polygon& a_Polygon, float a_Thickness) const
	{
		if (a_Polygon.positions.size() <= 1)
		{
			return;
		}

		std::vector<glm::vec2>::const_iterator current_it = a_Polygon.positions.begin();
		std::vector<glm::vec2>::const_iterator next_it = current_it + 1;

		while (next_it != a_Polygon.positions.end())
		{
			Line segment_line(*current_it, *next_it);
			Quad segment_quad = segment_line.ConstructQuad(a_Thickness);

			a_Target->AddQuad(
				segment_quad.ul, segment_quad.ur,
				segment_quad.ll, segment_quad.lr
			);

			++current_it;
			++next_it;
		}
	}

	void LineShape::_TriangulateOutlineGapless(MeshBuilder* a_Target, const Polygon& a_Polygon, float a_Thickness) const
	{
		if (a_Polygon.positions.size() <= 2)
		{
			return _TriangulateOutlineFast(a_Target, a_Polygon, a_Thickness);
		}

		std::vector<glm::vec2>::const_iterator previous_it = a_Polygon.positions.begin();
		std::vector<glm::vec2>::const_iterator current_it = previous_it + 1;
		std::vector<glm::vec2>::const_iterator next_it = current_it + 1;

		Line line_start(*previous_it, *current_it);
		Quad quad_start = line_start.ConstructQuad(a_Thickness);

		Quad quad_section;
		quad_section.ul = quad_start.ul;
		quad_section.ll = quad_start.ll;

		while (current_it != a_Polygon.positions.end())
		{
			const glm::vec2& previous = *previous_it;
			const glm::vec2& current = *current_it;

			Line line_previous(previous, current);
			Quad quad_previous = line_previous.ConstructQuad(a_Thickness);

			if (next_it == a_Polygon.positions.end())
			{
				a_Target->AddQuad(
					quad_section.ul, quad_previous.ur,
					quad_section.ll, quad_previous.lr
				);

				break;
			}

			const glm::vec2& next = *next_it;

			Line line_current(current, next);
			Quad quad_current = line_current.ConstructQuad(a_Thickness);

			Line line_joint(previous, next);

			bool segment_default = false;

			float side_joint = line_joint.GetCrossProduct(current);
			if (side_joint < 0.0f)
			{
				Line collision_line_previous(quad_previous.ll, quad_previous.lr);
				Line collision_line_next(quad_current.ll, quad_current.lr);

				Line::CollisionResult collision = collision_line_previous.Collides(collision_line_next);

				float outside_previous = collision_line_previous.GetCrossProduct(quad_current.lr);
				if (collision.time >= 0.0f && collision.time <= 1.0f && outside_previous > 0.0f)
				{
					a_Target->AddQuad(
						quad_section.ul, quad_previous.ur,
						quad_section.ll, collision.position
					);

					a_Target->AddTriangle(
						quad_previous.ur,
						collision.position,
						quad_current.ul
					);

					quad_section.ul = quad_current.ul;
					quad_section.ll = collision.position;
				}
				else
				{
					segment_default = true;
				}
			}
			else if (side_joint > 0.0f)
			{
				Line collision_line_previous(quad_previous.ul, quad_previous.ur);
				Line collision_line_next(quad_current.ul, quad_current.ur);

				Line::CollisionResult collision = collision_line_previous.Collides(collision_line_next);

				float outside_previous = collision_line_previous.GetCrossProduct(quad_current.ur);
				if (collision.time >= 0.0f && collision.time <= 1.0f && outside_previous < 0.0f)
				{
					a_Target->AddQuad(
						quad_section.ul, collision.position,
						quad_section.ll, quad_previous.lr
					);

					a_Target->AddTriangle(
						quad_previous.lr,
						quad_current.ll,
						collision.position
					);

					quad_section.ul = collision.position;
					quad_section.ll = quad_current.ll;
				}
				else
				{
					segment_default = true;
				}
			}
			else
			{
				segment_default = true;
			}

			if (segment_default)
			{
				a_Target->AddQuad(
					quad_section.ul, quad_previous.ur,
					quad_section.ll, quad_previous.lr
				);

				quad_section.ul = quad_current.ul;
				quad_section.ll = quad_current.ll;
			}

			if (current_it != a_Polygon.positions.begin())
			{
				previous_it++;
			}
			current_it++;
			if (next_it != a_Polygon.positions.end())
			{
				next_it++;
			}
		}
	}

}; // namespace ExLibris