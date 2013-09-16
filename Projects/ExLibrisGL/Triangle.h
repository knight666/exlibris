#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <glm/glm.hpp>

namespace ExLibris
{

	class Triangle
	{
	
	public:
	
		Triangle();
		Triangle(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C);

		bool IsConvex() const;

		bool IsPositionInside(const glm::vec2& a_Position) const;
	
	public:

		glm::vec2 a;
		glm::vec2 b;
		glm::vec2 c;
	
	}; // class Triangle

}; // namespace ExLibris

#endif