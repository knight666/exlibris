#ifndef _ISHAPEPATHVISITOR_H_
#define _ISHAPEPATHVISITOR_H_

#include <glm/glm.hpp>

namespace ExLibris
{

	class IShapePathVisitor
	{
	
	public:
	
		virtual ~IShapePathVisitor()
		{
		}
	
		virtual void VisitShapeStart() = 0;
		virtual void VisitShapePosition(const glm::vec2& a_Position) = 0;
		virtual void VisitShapeEnd() = 0;
	
	}; // class IShapePathVisitor

}; // namespace ExLibris

#endif