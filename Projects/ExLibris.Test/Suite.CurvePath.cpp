#include "ExLibris.Test.PCH.h"

#include "CurvePath.h"
#include "Mock.CurvePathVisitor.h"

using namespace ExLibris;

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

TEST(CurvePath, PathConicNoStartPoint)
{
	CurvePath path;
	path.ConicCurveTo(glm::vec2(-25.0f, 35.0f), glm::vec2(-50.0f, 50.0f));

	CurveSettings settings;
	settings.precision = 2;

	MockCurvePathVisitor visitor;
	path.Accept(visitor, settings);

	ASSERT_EQ(1, visitor.contours.size());
	ASSERT_EQ(2, visitor.contours[0].positions.size());
	EXPECT_FLOAT_EQ(-25.0f, visitor.contours[0].positions[0].x);
	EXPECT_FLOAT_EQ(30.0f, visitor.contours[0].positions[0].y);
	EXPECT_FLOAT_EQ(-50.0f, visitor.contours[0].positions[1].x);
	EXPECT_FLOAT_EQ(50.0f, visitor.contours[0].positions[1].y);
}

TEST(CurvePath, PathConicPrecisionHigh)
{
	CurvePath path;
	path.Move(glm::vec2(100.0f, 100.0f));
	path.ConicCurveTo(glm::vec2(50.0f, 50.0f), glm::vec2(100.0f, 25.0f));

	CurveSettings settings;
	settings.precision = 10;

	MockCurvePathVisitor visitor;
	path.Accept(visitor, settings);

	ASSERT_EQ(1, visitor.contours.size());
	ASSERT_EQ(11, visitor.contours[0].positions.size());
	EXPECT_FLOAT_EQ(100.0f, visitor.contours[0].positions[0].x);
	EXPECT_FLOAT_EQ(100.0f, visitor.contours[0].positions[0].y);
	EXPECT_FLOAT_EQ(75.0f, visitor.contours[0].positions[5].x);
	EXPECT_FLOAT_EQ(56.25f, visitor.contours[0].positions[5].y);
	EXPECT_FLOAT_EQ(100.0f, visitor.contours[0].positions[10].x);
	EXPECT_FLOAT_EQ(25.0f, visitor.contours[0].positions[10].y);
}

TEST(CurvePath, PathConicPrecisionOne)
{
	CurvePath path;
	path.Move(glm::vec2(36.0f, 21.0f));
	path.ConicCurveTo(glm::vec2(15.3f, 56.2f), glm::vec2(-30.0f, -88.8f));

	CurveSettings settings;
	settings.precision = 1;

	MockCurvePathVisitor visitor;
	path.Accept(visitor, settings);

	ASSERT_EQ(1, visitor.contours.size());
	ASSERT_EQ(2, visitor.contours[0].positions.size());
	EXPECT_FLOAT_EQ(36.0f, visitor.contours[0].positions[0].x);
	EXPECT_FLOAT_EQ(21.0f, visitor.contours[0].positions[0].y);
	EXPECT_FLOAT_EQ(-30.0f, visitor.contours[0].positions[1].x);
	EXPECT_FLOAT_EQ(-88.8f, visitor.contours[0].positions[1].y);
}

TEST(CurvePath, PathConicPrecisionNone)
{
	CurvePath path;
	path.Move(glm::vec2(22.4f, 87.3f));
	path.ConicCurveTo(glm::vec2(15.3f, 56.2f), glm::vec2(-30.0f, -88.8f));

	CurveSettings settings;
	settings.precision = 1;

	MockCurvePathVisitor visitor;
	path.Accept(visitor, settings);

	ASSERT_EQ(1, visitor.contours.size());
	ASSERT_EQ(2, visitor.contours[0].positions.size());
	EXPECT_FLOAT_EQ(22.4f, visitor.contours[0].positions[0].x);
	EXPECT_FLOAT_EQ(87.3f, visitor.contours[0].positions[0].y);
	EXPECT_FLOAT_EQ(-30.0f, visitor.contours[0].positions[1].x);
	EXPECT_FLOAT_EQ(-88.8f, visitor.contours[0].positions[1].y);
}

TEST(CurvePath, PathConicTwoCurves)
{
	CurvePath path;
	path.Move(glm::vec2(25.0f, 25.0f));
	path.ConicCurveTo(glm::vec2(50.0f, 25.0f), glm::vec2(100.0f, 25.0f));
	path.ConicCurveTo(glm::vec2(100.0f, 50.0f), glm::vec2(200.0f, 100.0f));

	CurveSettings settings;
	settings.precision = 3;

	MockCurvePathVisitor visitor;
	path.Accept(visitor, settings);

	ASSERT_EQ(1, visitor.contours.size());
	ASSERT_EQ(7, visitor.contours[0].positions.size());
	EXPECT_FLOAT_EQ(25.0f, visitor.contours[0].positions[0].x);
	EXPECT_FLOAT_EQ(25.0f, visitor.contours[0].positions[0].y);
	EXPECT_FLOAT_EQ(100.0f, visitor.contours[0].positions[3].x);
	EXPECT_FLOAT_EQ(25.0f, visitor.contours[0].positions[3].y);
	EXPECT_FLOAT_EQ(200.0f, visitor.contours[0].positions[6].x);
	EXPECT_FLOAT_EQ(100.0f, visitor.contours[0].positions[6].y);
}