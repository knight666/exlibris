#ifndef _ICURVEPATHVISITOR_H_
#define _ICURVEPATHVISITOR_H_

#include <glm/glm.hpp>

namespace ExLibris
{

	class ICurvePathVisitor
	{
	
	public:
	
		virtual ~ICurvePathVisitor()
		{
		}
	
		virtual void VisitShapeStart() = 0;
		virtual void VisitShapePosition(const glm::vec2& a_Position) = 0;
		virtual void VisitShapeEnd() = 0;
	
	}; // class ICurvePathVisitor

}; // namespace ExLibris

#endif