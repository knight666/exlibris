#ifndef _MESHOPENGL_H_
#define _MESHOPENGL_H_

#include <GL/glew.h>

#include <IMeshVisitor.h>

namespace Framework
{

	class MeshOpenGL
		: public ExLibris::IMeshVisitor
	{

	public:

		MeshOpenGL();
		~MeshOpenGL();

		ExLibris::TriangleOrientation GetOrientation() const;

		void VisitBuilderMeshBegin(unsigned int a_VertexCount);
		void VisitBuilderTriangle(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C);
		void VisitBuilderMeshEnd();

		GLuint GetBuffer() const;
		GLuint GetVertexCount() const;

	private:

		GLuint m_Buffer;
		glm::vec2* m_VertexData;
		size_t m_VertexCount;
		size_t m_VertexFilled;

	}; // class MeshOpenGL

}; // namespace Framework

#endif