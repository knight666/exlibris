#include "ExLibrisGL.PCH.h"

#include "LineShape.h"

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
		
		triangles->vertex_count = (polygon.positions.size() - 1) * 6;

		triangles->positions = new glm::vec2[triangles->vertex_count];

		glm::vec2* dst_position = triangles->positions;

		std::vector<glm::vec2>::const_iterator previous_it = polygon.positions.begin();
		std::vector<glm::vec2>::const_iterator current_it = previous_it + 1;
		std::vector<glm::vec2>::const_iterator next_it = current_it + 1;
		
		std::vector<ShapeType> shape_types;
		std::vector<glm::vec2> shape_positions;

		const glm::vec2& previous = *previous_it;
		const glm::vec2& current = *current_it;

		glm::vec2 world_up(0.0f, -1.0f);

		glm::vec2 start_normal = glm::normalize(current - previous);
		glm::vec2 start_position_upper;
		glm::vec2 start_position_lower;

		if (glm::dot(start_normal, world_up) > 0.0f)
		{
			start_position_upper = previous + glm::vec2(start_normal.y * thickness_half, -start_normal.x * thickness_half);
			start_position_lower = previous + glm::vec2(-start_normal.y * thickness_half, start_normal.x * thickness_half);
		}
		else
		{
			start_position_upper = previous + glm::vec2(-start_normal.y * thickness_half, start_normal.x * thickness_half);
			start_position_lower = previous + glm::vec2(start_normal.y * thickness_half, -start_normal.x * thickness_half);
		}

		if (start_position_upper.y > start_position_lower.y)
		{
			glm::vec2 swap = start_position_upper;
			start_position_upper = start_position_lower;
			start_position_lower = swap;
		}

		shape_positions.push_back(start_position_upper);
		shape_positions.push_back(start_position_lower);

		while (current_it != polygon.positions.end())
		{
			const glm::vec2& previous = *previous_it;
			const glm::vec2& current = *current_it;

			if (next_it == polygon.positions.end())
			{
				glm::vec2 end_position_upper;
				glm::vec2 end_position_lower;

				glm::vec2 end_normal = glm::normalize(current - previous);
				if (glm::dot(end_normal, world_up) > 0.0f)
				{
					end_position_upper = current + glm::vec2(end_normal.y * thickness_half, -end_normal.x * thickness_half);
					end_position_lower = current + glm::vec2(-end_normal.y * thickness_half, end_normal.x * thickness_half);
				}
				else
				{
					end_position_upper = current + glm::vec2(-end_normal.y * thickness_half, end_normal.x * thickness_half);
					end_position_lower = current + glm::vec2(end_normal.y * thickness_half, -end_normal.x * thickness_half);
				}

				shape_positions.push_back(end_position_upper);
				shape_positions.push_back(end_position_lower);

				shape_types.push_back(eShapeType_Quad);

				break;
			}
			
			const glm::vec2& next = *next_it;

			glm::vec2 next_offset_left;
			glm::vec2 next_offset_right;

			glm::vec2 normal_next = glm::normalize(next - current);
			if (glm::dot(normal_next, world_up) > 0.0f)
			{
				next_offset_left = glm::vec2(normal_next.y * thickness_half, -normal_next.x * thickness_half);
				next_offset_right = glm::vec2(-normal_next.y * thickness_half, normal_next.x * thickness_half);
			}
			else
			{
				next_offset_left = glm::vec2(-normal_next.y * thickness_half, normal_next.x * thickness_half);
				next_offset_right = glm::vec2(normal_next.y * thickness_half, -normal_next.x * thickness_half);
			}

			if (next_offset_left.y > next_offset_right.y)
			{
				glm::vec2 swap = next_offset_left;
				next_offset_left = next_offset_right;
				next_offset_right = swap;
			}

			glm::vec2 next_ul = current + next_offset_left;
			glm::vec2 next_ll = current + next_offset_right;
			glm::vec2 next_ur = next + next_offset_left;
			glm::vec2 next_lr = next + next_offset_right;

			glm::vec2 previous_offset_left;
			glm::vec2 previous_offset_right;

			glm::vec2 normal_previous = glm::normalize(current - previous);
			if (glm::dot(normal_previous, world_up) > 0.0f)
			{
				previous_offset_left = glm::vec2(normal_previous.y * thickness_half, -normal_previous.x * thickness_half);
				previous_offset_right = glm::vec2(-normal_previous.y * thickness_half, normal_previous.x * thickness_half);
			}
			else
			{
				previous_offset_left = glm::vec2(-normal_previous.y * thickness_half, normal_previous.x * thickness_half);
				previous_offset_right = glm::vec2(normal_previous.y * thickness_half, -normal_previous.x * thickness_half);
			}

			if (previous_offset_left.y > previous_offset_right.y)
			{
				glm::vec2 swap = previous_offset_left;
				previous_offset_left = previous_offset_right;
				previous_offset_right = swap;
			}

			glm::vec2 previous_ul = previous + previous_offset_left;
			glm::vec2 previous_ll = previous + previous_offset_right;
			glm::vec2 previous_ur = current + previous_offset_left;
			glm::vec2 previous_lr = current + previous_offset_right;

			CollisionResult collision_upper = LineCollision(
				previous_ul, previous_ur,
				next_ul, next_ur
			);

			CollisionResult collision_lower = LineCollision(
				previous_ll, previous_lr,
				next_ll, next_lr
			);

			if (collision_upper.time < collision_lower.time)
			{
				shape_positions.push_back(collision_upper.position);
				shape_positions.push_back(previous_lr);

				shape_types.push_back(eShapeType_Quad);

				shape_positions.push_back(next_ll);
				shape_positions.push_back(collision_upper.position);
			}
			else
			{
				shape_positions.push_back(collision_lower.position);
				shape_positions.push_back(previous_ur);

				shape_types.push_back(eShapeType_Quad);

				shape_positions.push_back(next_ul);
				shape_positions.push_back(collision_lower.position);
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