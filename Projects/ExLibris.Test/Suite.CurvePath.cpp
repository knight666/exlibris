#include "ExLibris.Test.PCH.h"

#include "CurvePath.h"

#include "Exception.h"

using namespace ExLibris;

TEST(CurvePath, Construct)
{
	CurvePath path;

	ASSERT_EQ(0, path.GetCommandCount());
	ASSERT_EQ(0, path.GetPositionCount());
}

TEST(CurvePath, Move)
{
	CurvePath path;

	path.Move(glm::vec2(-5.92f, 14.6f));

	ASSERT_EQ(1, path.GetPositionCount());
	EXPECT_VEC2_EQ(-5.92f, 14.6f, path.GetPosition(0));
}

TEST(CurvePath, LineTo)
{
	CurvePath path;

	path.LineTo(glm::vec2(5.87f, 14.93f));

	ASSERT_EQ(1, path.GetPositionCount());
	EXPECT_VEC2_EQ(5.87f, 14.93f, path.GetPosition(0));
}

TEST(CurvePath, ConicCurveTo)
{
	CurvePath path;

	path.ConicCurveTo(glm::vec2(100.5f, 55.9f), glm::vec2(33.56f, 145.0f));

	ASSERT_EQ(2, path.GetPositionCount());
	EXPECT_VEC2_EQ(100.5f, 55.9f, path.GetPosition(0));
	EXPECT_VEC2_EQ(33.56f, 145.0f, path.GetPosition(1));
}

TEST(CurvePath, QuadraticCurveTo)
{
	CurvePath path;

	path.QuadraticCurveTo(glm::vec2(0.25f, 2.5f), glm::vec2(3.5f, 2.44f), glm::vec2(-56.9f, 12.0f));

	ASSERT_EQ(3, path.GetPositionCount());
	EXPECT_VEC2_EQ(0.25f, 2.5f, path.GetPosition(0));
	EXPECT_VEC2_EQ(3.5f, 2.44f, path.GetPosition(1));
	EXPECT_VEC2_EQ(-56.9f, 12.0f, path.GetPosition(2));
}

TEST(CurvePath, GetPositionEmpty)
{
	CurvePath path;

	EXPECT_THROW(path.GetPosition(3), Exception);
}

TEST(CurvePath, GetPositionOutOfRange)
{
	CurvePath path;

	path.Move(glm::vec2(25.0f, 125.9f));

	EXPECT_THROW(path.GetPosition(3), Exception);
}

TEST(CurvePath, BuildMove)
{
	CurvePath path;
	path.Move(glm::vec2(33.3f, 15.5f));

	CurveSettings settings;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons[0].positions.size());
	EXPECT_VEC2_EQ(33.3f, 15.5f, polygons[0].positions[0]);
}

TEST(CurvePath, BuildMoveTwice)
{
	CurvePath path;
	path.Move(glm::vec2(66.78f, 91.91f));
	path.Move(glm::vec2(-56.1f, 71.8f));

	CurveSettings settings;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(2, polygons.size());
	ASSERT_EQ(1, polygons[0].positions.size());
	EXPECT_VEC2_EQ(66.78f, 91.91f, polygons[0].positions[0]);
	ASSERT_EQ(1, polygons[1].positions.size());
	EXPECT_VEC2_EQ(-56.1f, 71.8f, polygons[1].positions[0]);
}

TEST(CurvePath, BuildLine)
{
	CurvePath path;
	path.Move(glm::vec2(10.0f, 10.0f));
	path.LineTo(glm::vec2(25.0f, 10.0f));

	CurveSettings settings;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(2, polygons[0].positions.size());
	EXPECT_VEC2_EQ(10.0f, 10.0f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(25.0f, 10.0f, polygons[0].positions[1]);
}

TEST(CurvePath, BuildTwoLines)
{
	CurvePath path;
	path.Move(glm::vec2(6.0f, 16.0f));
	path.LineTo(glm::vec2(2.0f, 3.0f));
	path.LineTo(glm::vec2(17.0f, 17.0f));

	CurveSettings settings;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(3, polygons[0].positions.size());
	EXPECT_VEC2_EQ(6.0f, 16.0f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(2.0f, 3.0f, polygons[0].positions[1]);
	EXPECT_VEC2_EQ(17.0f, 17.0f, polygons[0].positions[2]);
}

TEST(CurvePath, BuildConic)
{
	CurvePath path;
	path.Move(glm::vec2(10.0f, 20.0f));
	path.ConicCurveTo(glm::vec2(15.0f, 15.0f), glm::vec2(20.0f, 20.0f));

	CurveSettings settings;
	settings.precision = 2;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(3, polygons[0].positions.size());
	EXPECT_VEC2_EQ(10.0f, 20.0f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(15.0f, 17.5f, polygons[0].positions[1]);
	EXPECT_VEC2_EQ(20.0f, 20.0f, polygons[0].positions[2]);
}

TEST(CurvePath, BuildConicNoStartPoint)
{
	CurvePath path;
	path.ConicCurveTo(glm::vec2(-25.0f, 35.0f), glm::vec2(-50.0f, 50.0f));

	CurveSettings settings;
	settings.precision = 2;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(2, polygons[0].positions.size());
	EXPECT_VEC2_EQ(-25.0f, 30.0f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(-50.0f, 50.0f, polygons[0].positions[1]);
}

TEST(CurvePath, BuildConicPrecisionHigh)
{
	CurvePath path;
	path.Move(glm::vec2(100.0f, 100.0f));
	path.ConicCurveTo(glm::vec2(50.0f, 50.0f), glm::vec2(100.0f, 25.0f));

	CurveSettings settings;
	settings.precision = 10;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(11, polygons[0].positions.size());
	EXPECT_VEC2_EQ(100.0f, 100.0f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(75.0f, 56.25f, polygons[0].positions[5]);
	EXPECT_VEC2_EQ(100.0f, 25.0f, polygons[0].positions[10]);
}

TEST(CurvePath, BuildConicPrecisionOne)
{
	CurvePath path;
	path.Move(glm::vec2(36.0f, 21.0f));
	path.ConicCurveTo(glm::vec2(15.3f, 56.2f), glm::vec2(-30.0f, -88.8f));

	CurveSettings settings;
	settings.precision = 1;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(2, polygons[0].positions.size());
	EXPECT_VEC2_EQ(36.0f, 21.0f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(-30.0f, -88.8f, polygons[0].positions[1]);
}

TEST(CurvePath, BuildConicPrecisionNone)
{
	CurvePath path;
	path.Move(glm::vec2(22.4f, 87.3f));
	path.ConicCurveTo(glm::vec2(15.3f, 56.2f), glm::vec2(-30.0f, -88.8f));

	CurveSettings settings;
	settings.precision = 0;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(2, polygons[0].positions.size());
	EXPECT_VEC2_EQ(22.4f, 87.3f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(-30.0f, -88.8f, polygons[0].positions[1]);
}

TEST(CurvePath, BuildConicTwoCurves)
{
	CurvePath path;
	path.Move(glm::vec2(25.0f, 25.0f));
	path.ConicCurveTo(glm::vec2(50.0f, 25.0f), glm::vec2(100.0f, 25.0f));
	path.ConicCurveTo(glm::vec2(100.0f, 50.0f), glm::vec2(200.0f, 100.0f));

	CurveSettings settings;
	settings.precision = 3;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(7, polygons[0].positions.size());
	EXPECT_VEC2_EQ(25.0f, 25.0f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(100.0f, 25.0f, polygons[0].positions[3]);
	EXPECT_VEC2_EQ(200.0f, 100.0f, polygons[0].positions[6]);
}

TEST(CurvePath, BuildQuadratic)
{
	CurvePath path;
	path.Move(glm::vec2(5.0f, 5.0f));
	path.QuadraticCurveTo(
		glm::vec2(5.0f, -25.0f),
		glm::vec2(25.0f, -25.0f),
		glm::vec2(25.0f, 5.0f)
	);

	CurveSettings settings;
	settings.precision = 4;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(5, polygons[0].positions.size());
	EXPECT_VEC2_EQ(5.0f, 5.0f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(8.125f, -11.875f, polygons[0].positions[1]);
	EXPECT_VEC2_EQ(15.0f, -17.5f, polygons[0].positions[2]);
	EXPECT_VEC2_EQ(21.875f, -11.875f, polygons[0].positions[3]);
	EXPECT_VEC2_EQ(25.0f, 5.0f, polygons[0].positions[4]);
}

TEST(CurvePath, BuildQuadraticNoStartPoint)
{
	CurvePath path;
	path.QuadraticCurveTo(
		glm::vec2(15.0f, 26.0f),
		glm::vec2(33.4f, 19.2f),
		glm::vec2(-12.0f, 29.9f)
	);

	CurveSettings settings;
	settings.precision = 3;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(3, polygons[0].positions.size());
	EXPECT_VEC2_EQ(-12.0f, 29.9f, polygons[0].positions[2]);
}

TEST(CurvePath, BuildQuadraticPrecisionHigh)
{
	CurvePath path;
	path.Move(glm::vec2(24.0f, 87.2f));
	path.QuadraticCurveTo(
		glm::vec2(15.9f, 33.1f),
		glm::vec2(18.9f, 118.2f),
		glm::vec2(33.91f, 88.91f)
	);

	CurveSettings settings;
	settings.precision = 16;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(17, polygons[0].positions.size());
	EXPECT_VEC2_EQ(24.0f, 87.2f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(19.618299f, 71.68959f, polygons[0].positions[6]);
	EXPECT_VEC2_EQ(33.91f, 88.91f, polygons[0].positions[16]);
}

TEST(CurvePath, BuildQuadraticPrecisionOne)
{
	CurvePath path;
	path.Move(glm::vec2(57.36f, 14.25f));
	path.QuadraticCurveTo(
		glm::vec2(-65.32f, -478.25f),
		glm::vec2(-53.24f, 45.36f),
		glm::vec2(63.57f, 49.886f)
	);

	CurveSettings settings;
	settings.precision = 1;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(2, polygons[0].positions.size());
	EXPECT_VEC2_EQ(57.36f, 14.25f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(63.57f, 49.886f, polygons[0].positions[1]);
}

TEST(CurvePath, BuildQuadraticPrecisionTwoCurves)
{
	CurvePath path;
	path.Move(glm::vec2(222.8f, 34.9f));
	path.QuadraticCurveTo(
		glm::vec2(12.87f, 912.9f),
		glm::vec2(15.9f, 99.1f),
		glm::vec2(88.8812f, -12.9f)
	);
	path.QuadraticCurveTo(
		glm::vec2(67.9f, 15.98f),
		glm::vec2(-98.12f, -54.31f),
		glm::vec2(16.87f, 912.08f)
	);

	CurveSettings settings;
	settings.precision = 14;

	std::vector<Polygon> polygons = path.BuildPolygons(settings);

	ASSERT_EQ(1, polygons.size());
	ASSERT_EQ(29, polygons[0].positions.size());
	EXPECT_VEC2_EQ(222.8f, 34.9f, polygons[0].positions[0]);
	EXPECT_VEC2_EQ(88.8812f, -12.9f, polygons[0].positions[14]);
	EXPECT_VEC2_EQ(16.87f, 912.08f, polygons[0].positions[28]);
}