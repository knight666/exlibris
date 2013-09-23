#ifndef _LINE_H_
#define _LINE_H_

#include <glm/glm.hpp>

#include "Quad.h"

namespace ExLibris
{

	class Line
	{

	public:

		Line();
		Line(const glm::vec2& a_Start, const glm::vec2& a_End);

		Quad ConstructQuad(float a_Thickness) const;

	public:
	
		glm::vec2 start;
		glm::vec2 end;
	
	}; // class Line

}; // namespace ExLibris

#endif