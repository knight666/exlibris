#include "ExLibrisGL.PCH.h"

#include "Quad.h"

namespace ExLibris
{

	Quad::Quad()
	{
	}

	Quad::Quad(
		const glm::vec2& a_UpperLeft, const glm::vec2& a_UpperRight,
		const glm::vec2& a_LowerLeft, const glm::vec2& a_LowerRight
	)
		: ul(a_UpperLeft)
		, ur(a_UpperRight)
		, ll(a_LowerLeft)
		, lr(a_LowerRight)
	{
	}

}; // namespace ExLibris