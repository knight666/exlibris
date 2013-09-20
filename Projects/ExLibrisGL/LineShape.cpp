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

		std::vector<glm::vec2>::const_iterator position_it = polygon.positions.begin();
		std::vector<glm::vec2>::const_iterator position_next_it = position_it + 1;
		
		while (position_next_it != polygon.positions.end())
		{
			const glm::vec2& current = *position_it++;
			const glm::vec2& next = *position_next_it++;

			// 1 ---------- 0
			// |          / |
			// |       /    |
			// c .......... n
			// |    /       |
			// | /          |
			// 2 ---------- 3

			glm::vec2 normal = glm::normalize(next - current);
			glm::vec2 offset_left(normal.y * a_Thickness, -normal.x * thickness_half);
			glm::vec2 offset_right(-normal.y * a_Thickness, normal.x * thickness_half);
			
			dst_position[0] = next + offset_left;
			dst_position[1] = current + offset_left;
			dst_position[2] = current + offset_right;

			dst_position[3] = next + offset_left;
			dst_position[4] = current + offset_right;
			dst_position[5] = next + offset_right;
			
			dst_position += 6;
		}

		return triangles;
	}

}; // namespace ExLibris