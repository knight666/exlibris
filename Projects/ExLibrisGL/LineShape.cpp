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

	struct CollisionResult
	{
		CollisionResult()
			: collides(false)
			, time(0.0f)
		{
		}

		bool collides;
		float time;
		glm::vec2 position;
	};

	CollisionResult LineCollision(const glm::vec2& a_StartA, const glm::vec2& a_EndA, const glm::vec2& a_StartB, const glm::vec2& a_EndB)
	{
		CollisionResult result;

		glm::vec2 delta_a = a_EndA - a_StartA;
		glm::vec2 delta_b = a_EndB - a_StartB;

		float b_dot_d_perp = (delta_a.x * delta_b.y) - (delta_a.y * delta_b.x);
		if (b_dot_d_perp == 0.0f)
		{
			return result;
		}

		result.collides = true;

		glm::vec2 local = a_StartB - a_StartA;
		result.time = ((local.x * delta_b.y) - (local.y * delta_b.x)) / b_dot_d_perp;
		result.position = a_StartA + (delta_a * result.time);

		return result;
	}

	TriangleList* LineShape::Triangulate(float a_Thickness) const
	{
		const Polygon& polygon = *m_Polygons.begin();

		if (polygon.positions.size() < 2)
		{
			return nullptr;
		}

		float thickness_half = a_Thickness / 2.0f;

		TriangleList* triangles = new TriangleList;
		
		size_t section_count = polygon.positions.size() - 1;
		size_t corner_count = section_count - 1;

		triangles->vertex_count = (section_count * 6) + (corner_count * 3);

		triangles->positions = new glm::vec2[triangles->vertex_count];

		glm::vec2* dst_position = triangles->positions;

		std::vector<glm::vec2>::const_iterator previous_it = polygon.positions.begin();
		std::vector<glm::vec2>::const_iterator current_it = previous_it + 1;
		std::vector<glm::vec2>::const_iterator next_it = current_it + 1;
		
		std::vector<ShapeType> shape_types;
		std::vector<glm::vec2> shape_positions;

		glm::vec2 world_up(0.0f, -1.0f);

		Line line_start(*previous_it, *current_it);
		Quad quad_start = line_start.ConstructQuad(a_Thickness);
		
		shape_positions.push_back(quad_start.ul);
		shape_positions.push_back(quad_start.ll);

		while (current_it != polygon.positions.end())
		{
			const glm::vec2& previous = *previous_it;
			const glm::vec2& current = *current_it;

			Line line_previous(previous, current);
			Quad quad_previous = line_previous.ConstructQuad(a_Thickness);

			if (next_it == polygon.positions.end())
			{
				shape_positions.push_back(quad_previous.ur);
				shape_positions.push_back(quad_previous.lr);

				shape_types.push_back(eShapeType_Quad);

				break;
			}
			
			const glm::vec2& next = *next_it;

			Line line_current(current, next);
			Quad quad_current = line_current.ConstructQuad(a_Thickness);

			Line line_joint(previous, next);

			float side_joint = line_joint.GetCrossProduct(current);
			if (side_joint < 0.0f)
			{
				Line collision_line_previous(quad_previous.ll, quad_previous.lr);
				Line collision_line_next(quad_current.ll, quad_current.lr);

				Line::CollisionResult collision = collision_line_previous.Collides(collision_line_next);

				shape_positions.push_back(quad_previous.ur);
				shape_positions.push_back(collision.position);

				shape_types.push_back(eShapeType_Quad);

				shape_positions.push_back(quad_previous.ur);
				shape_positions.push_back(quad_current.ul);
				shape_positions.push_back(collision.position);

				shape_types.push_back(eShapeType_Triangle);

				shape_positions.push_back(quad_current.ul);
				shape_positions.push_back(collision.position);
			}
			else
			{
				Line collision_line_previous(quad_previous.ul, quad_previous.ur);
				Line collision_line_next(quad_current.ul, quad_current.ur);

				Line::CollisionResult collision = collision_line_previous.Collides(collision_line_next);

				shape_positions.push_back(collision.position);
				shape_positions.push_back(quad_previous.lr);

				shape_types.push_back(eShapeType_Quad);

				shape_positions.push_back(quad_previous.lr);
				shape_positions.push_back(quad_current.ll);
				shape_positions.push_back(collision.position);

				shape_types.push_back(eShapeType_Triangle);

				shape_positions.push_back(collision.position);
				shape_positions.push_back(quad_current.ll);
			}

			if (current_it != polygon.positions.begin())
			{
				previous_it++;
			}
			current_it++;
			if (next_it != polygon.positions.end())
			{
				next_it++;
			}
		}

		std::vector<glm::vec2>::iterator position_it = shape_positions.begin();
		for (std::vector<ShapeType>::iterator type_it = shape_types.begin(); type_it != shape_types.end(); ++type_it)
		{
			if (*type_it == eShapeType_Triangle)
			{
				dst_position[0] = *position_it++;
				dst_position[1] = *position_it++;
				dst_position[2] = *position_it++;

				dst_position += 3;
			}
			else if (*type_it == eShapeType_Quad)
			{
				glm::vec2 quad_ul = *position_it++;
				glm::vec2 quad_ll = *position_it++;
				glm::vec2 quad_ur = *position_it++;
				glm::vec2 quad_lr = *position_it++;

				dst_position[0] = quad_ur;
				dst_position[1] = quad_ul;
				dst_position[2] = quad_ll;

				dst_position[3] = quad_ur;
				dst_position[4] = quad_ll;
				dst_position[5] = quad_lr;

				dst_position += 6;
			}
		}

		return triangles;
	}

}; // namespace ExLibris