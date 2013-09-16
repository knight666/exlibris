#include "ExLibrisGL.PCH.h"

#include "Polygon.h"

#include "Triangle.h"

namespace ExLibris
{

	Polygon::Polygon()
	{
	}
	
	Polygon::~Polygon()
	{
		m_Shapes.clear();
	}

	void Polygon::AddShape(const Shape& a_Shape)
	{
		m_Shapes.push_back(a_Shape);
	}

	TriangleList Polygon::Triangulate() const
	{
		TriangleList triangles;

		for (std::vector<Shape>::const_iterator shape_it = m_Shapes.begin(); shape_it != m_Shapes.end(); ++shape_it)
		{
			const Shape& shape = *shape_it;

			int n = (int)shape.positions.size();
			if (n < 3)
			{
				continue;
			}

			int* avl_data = new int[n];
			for (int index = 0; index < n; ++index)
			{
				avl_data[index] = index;
			}
			int* avl = avl_data;

			int i = 0;
			int al = n;
			while (al > 3)
			{
				int i0 = avl[(i + 0) % al];
				int i1 = avl[(i + 1) % al];
				int i2 = avl[(i + 2) % al];

				Triangle triangulate(shape.positions[i0], shape.positions[i1], shape.positions[i2]);

				bool earFound = false;

				if (triangulate.IsConvex())
				{
					earFound = true;
					for (int j = 0; j < al; ++j)
					{
						int vi = avl[j];
						if (vi == i0 || vi == i1 || vi == i2)
						{
							continue;
						}

						if (triangulate.IsPositionInside(shape.positions[vi]))
						{
							earFound = false;

							break;
						}
					}
				}

				if (earFound)
				{
					triangles.positions.push_back(triangulate.a);
					triangles.positions.push_back(triangulate.b);
					triangles.positions.push_back(triangulate.c);

					avl += (i + 1) % al;
					al--;
					i = 0;
				}
				else
				{
					if ((i + 1) > (3 * al))
					{
						break;
					}

					i++;
				}
			}

			int i0 = avl[(i + 0) % al];
			triangles.positions.push_back(shape.positions[i0]);

			int i1 = avl[(i + 1) % al];
			triangles.positions.push_back(shape.positions[i1]);

			int i2 = avl[(i + 2) % al];
			triangles.positions.push_back(shape.positions[i2]);

			delete avl_data;
		}

		return triangles;
	}

	bool Polygon::_IsConvex(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C) const
	{
		return (((a_A.y - a_B.y) * (a_C.x - a_B.x)) + ((a_B.x - a_A.x) * (a_C.y - a_B.y))) >= 0;
	}

	bool Polygon::_IsPointInTriangle(const glm::vec2& a_TriangleA, const glm::vec2& a_TriangleB, const glm::vec2& a_TriangleC, const glm::vec2& a_Position) const
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