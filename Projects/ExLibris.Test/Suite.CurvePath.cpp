#include "ExLibris.Test.PCH.h"

#include "CurvePath.h"

using namespace ExLibris;

class MockCurvePathVisitor
	: public ICurvePathVisitor
{

public:

	~MockCurvePathVisitor()
	{
	}

	void VisitShapeStart()
	{
		Contour contour;
		contours.push_back(contour);
	}

	void VisitShapePosition(const glm::vec2& a_Position)
	{
		Contour& contour = contours.back();
		contour.positions.push_back(a_Position);
	}

	void VisitShapeEnd()
	{
	}

public:

	struct Contour
	{
		std::vector<glm::vec2> positions;
	};
	std::vector<Contour> contours;

};

TEST(CurvePath, PathLine)
{
	CurvePath path;
	path.Move(glm::vec2(10.0f, 10.0f));
	path.LineTo(glm::vec2(25.0f, 10.0f));

	MockCurvePathVisitor visitor;
	path.Accept(visitor);

	ASSERT_EQ(1, visitor.contours.size());
	ASSERT_EQ(2, visitor.contours[0].positions.size());
	EXPECT_FLOAT_EQ(10.0f, visitor.contours[0].positions[0].x);
	EXPECT_FLOAT_EQ(10.0f, visitor.contours[0].positions[0].y);
	EXPECT_FLOAT_EQ(25.0f, visitor.contours[0].positions[1].x);
	EXPECT_FLOAT_EQ(10.0f, visitor.contours[0].positions[1].y);
}