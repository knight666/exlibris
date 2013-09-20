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
		
		std::vector<glm::vec2> quad_positions;

		glm::vec2 start_normal = glm::normalize(*current_it - *previous_it);
		glm::vec2 start_position_upper = *previous_it + glm::vec2(-start_normal.y * thickness_half, start_normal.x * thickness_half);
		glm::vec2 start_position_lower = *previous_it + glm::vec2(start_normal.y * thickness_half, -start_normal.x * thickness_half);

		quad_positions.push_back(start_position_upper);
		quad_positions.push_back(start_position_lower);

		while (current_it != polygon.positions.end())
		{
			const glm::vec2& previous = *previous_it;
			const glm::vec2& current = *current_it;

			if (next_it == polygon.positions.end())
			{
				glm::vec2 end_normal = glm::normalize(current - previous);
				glm::vec2 end_position_upper = current + glm::vec2(-end_normal.y * thickness_half, end_normal.x * thickness_half);
				glm::vec2 end_position_lower = current + glm::vec2(end_normal.y * thickness_half, -end_normal.x * thickness_half);

				quad_positions.push_back(end_position_upper);
				quad_positions.push_back(end_position_lower);

				break;
			}
			
			const glm::vec2& next = *next_it;

			glm::vec2 normal_next = glm::normalize(next - current);

			glm::vec2 next_offset_left(-normal_next.y * thickness_half, normal_next.x * thickness_half);
			glm::vec2 next_offset_right(normal_next.y * thickness_half, -normal_next.x * thickness_half);

			glm::vec2 next_ul = current + next_offset_left;
			glm::vec2 next_ll = current + next_offset_right;
			glm::vec2 next_ur = next + next_offset_left;
			glm::vec2 next_lr = next + next_offset_right;

			glm::vec2 normal_previous = glm::normalize(current - previous);

			glm::vec2 previous_offset_left(-normal_previous.y * thickness_half, normal_previous.x * thickness_half);
			glm::vec2 previous_offset_right(normal_previous.y * thickness_half, -normal_previous.x * thickness_half);

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
				quad_positions.push_back(collision_upper.position);
				quad_positions.push_back(previous_lr);

				quad_positions.push_back(next_ll);
				quad_positions.push_back(collision_upper.position);
			}
			else
			{
				quad_positions.push_back(collision_lower.position);
				quad_positions.push_back(previous_ur);
					
				quad_positions.push_back(next_ul);
				quad_positions.push_back(collision_lower.position);
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

		for (std::vector<glm::vec2>::iterator quad_it = quad_positions.begin(); quad_it != quad_positions.end();)
		{
			glm::vec2 quad_ul = *quad_it++;
			glm::vec2 quad_ll = *quad_it++;
			glm::vec2 quad_ur = *quad_it++;
			glm::vec2 quad_lr = *quad_it++;

			dst_position[0] = quad_ur;
			dst_position[1] = quad_ul;
			dst_position[2] = quad_ll;

			dst_position[3] = quad_ur;
			dst_position[4] = quad_ll;
			dst_position[5] = quad_lr;

			dst_position += 6;
		}

		return triangles;
	}

}; // namespace ExLibris