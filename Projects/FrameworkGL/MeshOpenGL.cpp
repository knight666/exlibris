#include "FrameworkGL.PCH.h"

#include "MeshOpenGL.h"

namespace Framework
{

	MeshOpenGL::MeshOpenGL()
		: m_Buffer(0)
		, m_VertexData(nullptr)
		, m_VertexCount(0)
		, m_VertexFilled(0)
	{
		glGenBuffers(1, &m_Buffer);
	}

	MeshOpenGL::~MeshOpenGL()
	{
		if (m_VertexData != nullptr)
		{
			delete [] m_VertexData;
			m_VertexData = nullptr;
		}

		glDeleteBuffers(1, &m_Buffer);
	}

	ExLibris::TriangleOrientation MeshOpenGL::GetOrientation() const
	{
		return ExLibris::eTriangleOrientation_CounterClockWise;
	}

	void MeshOpenGL::VisitBuilderMeshBegin(unsigned int a_VertexCount)
	{
		m_VertexFilled = 0;

		if (a_VertexCount > m_VertexCount)
		{
			m_VertexCount = a_VertexCount;

			if (m_VertexData != nullptr)
			{
				delete [] m_VertexData;
			}
			m_VertexData = new glm::vec2[m_VertexCount];
		}
	}

	void MeshOpenGL::VisitBuilderTriangle(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C)
	{
		m_VertexData[m_VertexFilled++] = a_A;
		m_VertexData[m_VertexFilled++] = a_B;
		m_VertexData[m_VertexFilled++] = a_C;
	}

	void MeshOpenGL::VisitBuilderMeshEnd()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
		glBufferData(GL_ARRAY_BUFFER, m_VertexFilled * sizeof(glm::vec2), m_VertexData, GL_STATIC_DRAW);
	}

	void MeshOpenGL::Render()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
		glDrawArrays(GL_TRIANGLES, 0, m_VertexFilled);
	}

}; // namespace Framework