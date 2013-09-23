#ifndef _QUAD_H_
#define _QUAD_H_

#include <glm/glm.hpp>

namespace ExLibris
{

	class Quad
	{
	
	public:
	
		Quad();
		Quad(
			const glm::vec2& a_UpperLeft, const glm::vec2& a_UpperRight,
			const glm::vec2& a_LowerLeft, const glm::vec2& a_LowerRight
		);
	
	public:

		glm::vec2 ul;
		glm::vec2 ur;
		glm::vec2 ll;
		glm::vec2 lr;
	
	}; // class Quad

}; // namespace ExLibris

#endif