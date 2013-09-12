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

TEST(CurvePath, PathMove)
{
	CurvePath path;
	path.Move(glm::vec2(33.3f, 15.5f));

	CurveSettings settings;

	MockCurvePathVisitor visitor;
	path.Accept(visitor, settings);

	ASSERT_EQ(1, visitor.contours.size());
	ASSERT_EQ(1, visitor.contours[0].positions.size());
	EXPECT_FLOAT_EQ(33.3f, visitor.contours[0].positions[0].x);
	EXPECT_FLOAT_EQ(15.5f, visitor.contours[0].positions[0].y);
}

TEST(CurvePath, PathLine)
{
	CurvePath path;
	path.Move(glm::vec2(10.0f, 10.0f));
	path.LineTo(glm::vec2(25.0f, 10.0f));

	CurveSettings settings;

	MockCurvePathVisitor visitor;
	path.Accept(visitor, settings);

	ASSERT_EQ(1, visitor.contours.size());
	ASSERT_EQ(2, visitor.contours[0].positions.size());
	EXPECT_FLOAT_EQ(10.0f, visitor.contours[0].positions[0].x);
	EXPECT_FLOAT_EQ(10.0f, visitor.contours[0].positions[0].y);
	EXPECT_FLOAT_EQ(25.0f, visitor.contours[0].positions[1].x);
	EXPECT_FLOAT_EQ(10.0f, visitor.contours[0].positions[1].y);
}

TEST(CurvePath, PathTwoLines)
{
	CurvePath path;
	path.Move(glm::vec2(6.0f, 16.0f));
	path.LineTo(glm::vec2(2.0f, 3.0f));
	path.LineTo(glm::vec2(17.0f, 17.0f));

	CurveSettings settings;

	MockCurvePathVisitor visitor;
	path.Accept(visitor, settings);

	ASSERT_EQ(1, visitor.contours.size());
	ASSERT_EQ(3, visitor.contours[0].positions.size());
	EXPECT_FLOAT_EQ(6.0f, visitor.contours[0].positions[0].x);
	EXPECT_FLOAT_EQ(16.0f, visitor.contours[0].positions[0].y);
	EXPECT_FLOAT_EQ(2.0f, visitor.contours[0].positions[1].x);
	EXPECT_FLOAT_EQ(3.0f, visitor.contours[0].positions[1].y);
	EXPECT_FLOAT_EQ(17.0f, visitor.contours[0].positions[2].x);
	EXPECT_FLOAT_EQ(17.0f, visitor.contours[0].positions[2].y);
}

TEST(CurvePath, PathConic)
{
	CurvePath path;
	path.Move(glm::vec2(10.0f, 20.0f));
	path.ConicCurveTo(glm::vec2(15.0f, 15.0f), glm::vec2(20.0f, 20.0f));

	CurveSettings settings;
	settings.precision = 2;

	MockCurvePathVisitor visitor;
	path.Accept(visitor, settings);

	ASSERT_EQ(1, visitor.contours.size());
	ASSERT_EQ(3, visitor.contours[0].positions.size());
	EXPECT_FLOAT_EQ(10.0f, visitor.contours[0].positions[0].x);
	EXPECT_FLOAT_EQ(20.0f, visitor.contours[0].positions[0].y);
	EXPECT_FLOAT_EQ(15.0f, visitor.contours[0].positions[1].x);
	EXPECT_FLOAT_EQ(17.5f, visitor.contours[0].positions[1].y);
	EXPECT_FLOAT_EQ(20.0f, visitor.contours[0].positions[2].x);
	EXPECT_FLOAT_EQ(20.0f, visitor.contours[0].positions[2].y);
}