#ifndef _MOCK_CURVEPATHVISITOR_H_
#define _MOCK_CURVEPATHVISITOR_H_

#include "ICurvePathVisitor.h"

namespace ExLibris
{

	class MockCurvePathVisitor
		: public ICurvePathVisitor
	{

	public:

		~MockCurvePathVisitor()
		{
		}

		void VisitCurveStart()
		{
			Contour contour;
			contours.push_back(contour);
		}

		void VisitCurvePosition(const glm::vec2& a_Position)
		{
			Contour& contour = contours.back();
			contour.positions.push_back(a_Position);
		}

		void VisitCurveEnd()
		{
		}

	public:

		struct Contour
		{
			std::vector<glm::vec2> positions;
		};
		std::vector<Contour> contours;

	};

}; // namespace ExLibris

#endif