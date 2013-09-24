#ifndef _LINE_H_
#define _LINE_H_

#include <glm/glm.hpp>

#include "Quad.h"

namespace ExLibris
{

	class Line
	{

	public:

		struct CollisionResult
		{
			bool collides;
			float time;
			glm::vec2 position;
		};

	public:

		Line();
		Line(const glm::vec2& a_Start, const glm::vec2& a_End);

		float GetPerpendicularDistanceToPosition(const glm::vec2& a_Position) const;

		Quad ConstructQuad(float a_Thickness) const;

		CollisionResult Collides(const Line& a_Other) const;

	public:
	
		glm::vec2 start;
		glm::vec2 end;
	
	}; // class Line

}; // namespace ExLibris

#endif