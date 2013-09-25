#ifndef _IMESHVISITOR_H_
#define _IMESHVISITOR_H_

#include "TriangleOrientation.h"

#include <glm/glm.hpp>

namespace ExLibris
{

	class IMeshVisitor
	{
	
	public:
	
		virtual ~IMeshVisitor()
		{
		}
	
		virtual TriangleOrientation GetOrientation() const = 0;

		virtual void VisitBuilderMesh(unsigned int a_VertexCount) = 0;
		virtual void VisitBuilderAddTriangle(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C) = 0;
	
	}; // class IMeshVisitor

}; // namespace ExLibris

#endif