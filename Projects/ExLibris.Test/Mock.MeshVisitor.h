#ifndef _MOCK_MESHVISITOR_H_
#define _MOCK_MESHVISITOR_H_

#include <IMeshVisitor.h>
#include <Triangle.h>

namespace ExLibris
{

	class MockMeshVisitor
		: public IMeshVisitor
	{
	
	public:

		MockMeshVisitor()
			: vertex_count(0)
			, mesh_count(0)
		{
		}
	
		TriangleOrientation GetOrientation() const
		{
			return eTriangleOrientation_CounterClockWise;
		}

		void VisitBuilderMeshBegin(unsigned int a_VertexCount)
		{
			vertex_count += a_VertexCount;
		}

		void VisitBuilderTriangle(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C)
		{
			triangles.push_back(Triangle(a_A, a_B, a_C));
		}

		void VisitBuilderMeshEnd()
		{
			mesh_count++;
		}

	public:

		unsigned int vertex_count;
		std::vector<Triangle> triangles;
		unsigned int mesh_count;
	
	}; // class MockMeshVisitor

}

#endif