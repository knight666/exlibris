#include "ExLibris.Test.PCH.h"

#include "CurvePath.h"

using namespace ExLibris;

TEST(CurvePath, BuildPolygon)
{
	CurvePath path;
	path.Move(glm::vec2(-56.9f, 12.0f));
	path.LineTo(glm::vec2(123.9f, 76.3f));
	path.LineTo(glm::vec2(26.5f, 99.8f));

	CurveSettings settings;

	Polygon* shape = path.BuildPolygon(settings);

	ASSERT_NE(nullptr, shape);
	ASSERT_EQ(3, shape->positions.size());
	EXPECT_VEC2_EQ(-56.9f, 12.0f, shape->positions[0]);
	EXPECT_VEC2_EQ(123.9f, 76.3f, shape->positions[1]);
	EXPECT_VEC2_EQ(26.5f, 99.8f, shape->positions[2]);
}

TEST(CurvePath, PathMove)
{
	CurvePath path;
	path.Move(glm::vec2(33.3f, 15.5f));

	CurveSettings settings;

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(1, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(33.3f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(15.5f, shapes[0].positions[0].y);
}

TEST(CurvePath, PathMoveTwice)
{
	CurvePath path;
	path.Move(glm::vec2(66.78f, 91.91f));
	path.Move(glm::vec2(-56.1f, 71.8f));

	CurveSettings settings;

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(2, shapes.size());
	ASSERT_EQ(1, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(66.78f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(91.91f, shapes[0].positions[0].y);
	ASSERT_EQ(1, shapes[1].positions.size());
	EXPECT_FLOAT_EQ(-56.1f, shapes[1].positions[0].x);
	EXPECT_FLOAT_EQ(71.8f, shapes[1].positions[0].y);
}

TEST(CurvePath, PathLine)
{
	CurvePath path;
	path.Move(glm::vec2(10.0f, 10.0f));
	path.LineTo(glm::vec2(25.0f, 10.0f));

	CurveSettings settings;

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(2, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(10.0f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(10.0f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(25.0f, shapes[0].positions[1].x);
	EXPECT_FLOAT_EQ(10.0f, shapes[0].positions[1].y);
}

TEST(CurvePath, PathTwoLines)
{
	CurvePath path;
	path.Move(glm::vec2(6.0f, 16.0f));
	path.LineTo(glm::vec2(2.0f, 3.0f));
	path.LineTo(glm::vec2(17.0f, 17.0f));

	CurveSettings settings;

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(3, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(6.0f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(16.0f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(2.0f, shapes[0].positions[1].x);
	EXPECT_FLOAT_EQ(3.0f, shapes[0].positions[1].y);
	EXPECT_FLOAT_EQ(17.0f, shapes[0].positions[2].x);
	EXPECT_FLOAT_EQ(17.0f, shapes[0].positions[2].y);
}

TEST(CurvePath, PathConic)
{
	CurvePath path;
	path.Move(glm::vec2(10.0f, 20.0f));
	path.ConicCurveTo(glm::vec2(15.0f, 15.0f), glm::vec2(20.0f, 20.0f));

	CurveSettings settings;
	settings.precision = 2;

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(3, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(10.0f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(20.0f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(15.0f, shapes[0].positions[1].x);
	EXPECT_FLOAT_EQ(17.5f, shapes[0].positions[1].y);
	EXPECT_FLOAT_EQ(20.0f, shapes[0].positions[2].x);
	EXPECT_FLOAT_EQ(20.0f, shapes[0].positions[2].y);
}

TEST(CurvePath, PathConicNoStartPoint)
{
	CurvePath path;
	path.ConicCurveTo(glm::vec2(-25.0f, 35.0f), glm::vec2(-50.0f, 50.0f));

	CurveSettings settings;
	settings.precision = 2;

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(2, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(-25.0f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(30.0f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(-50.0f, shapes[0].positions[1].x);
	EXPECT_FLOAT_EQ(50.0f, shapes[0].positions[1].y);
}

TEST(CurvePath, PathConicPrecisionHigh)
{
	CurvePath path;
	path.Move(glm::vec2(100.0f, 100.0f));
	path.ConicCurveTo(glm::vec2(50.0f, 50.0f), glm::vec2(100.0f, 25.0f));

	CurveSettings settings;
	settings.precision = 10;

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(11, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(100.0f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(100.0f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(75.0f, shapes[0].positions[5].x);
	EXPECT_FLOAT_EQ(56.25f, shapes[0].positions[5].y);
	EXPECT_FLOAT_EQ(100.0f, shapes[0].positions[10].x);
	EXPECT_FLOAT_EQ(25.0f, shapes[0].positions[10].y);
}

TEST(CurvePath, PathConicPrecisionOne)
{
	CurvePath path;
	path.Move(glm::vec2(36.0f, 21.0f));
	path.ConicCurveTo(glm::vec2(15.3f, 56.2f), glm::vec2(-30.0f, -88.8f));

	CurveSettings settings;
	settings.precision = 1;

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(2, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(36.0f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(21.0f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(-30.0f, shapes[0].positions[1].x);
	EXPECT_FLOAT_EQ(-88.8f, shapes[0].positions[1].y);
}

TEST(CurvePath, PathConicPrecisionNone)
{
	CurvePath path;
	path.Move(glm::vec2(22.4f, 87.3f));
	path.ConicCurveTo(glm::vec2(15.3f, 56.2f), glm::vec2(-30.0f, -88.8f));

	CurveSettings settings;
	settings.precision = 1;

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(2, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(22.4f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(87.3f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(-30.0f, shapes[0].positions[1].x);
	EXPECT_FLOAT_EQ(-88.8f, shapes[0].positions[1].y);
}

TEST(CurvePath, PathConicTwoCurves)
{
	CurvePath path;
	path.Move(glm::vec2(25.0f, 25.0f));
	path.ConicCurveTo(glm::vec2(50.0f, 25.0f), glm::vec2(100.0f, 25.0f));
	path.ConicCurveTo(glm::vec2(100.0f, 50.0f), glm::vec2(200.0f, 100.0f));

	CurveSettings settings;
	settings.precision = 3;

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(7, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(25.0f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(25.0f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(100.0f, shapes[0].positions[3].x);
	EXPECT_FLOAT_EQ(25.0f, shapes[0].positions[3].y);
	EXPECT_FLOAT_EQ(200.0f, shapes[0].positions[6].x);
	EXPECT_FLOAT_EQ(100.0f, shapes[0].positions[6].y);
}

TEST(CurvePath, PathQuadratic)
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

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(5, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(5.0f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(5.0f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(8.125f, shapes[0].positions[1].x);
	EXPECT_FLOAT_EQ(-11.875f, shapes[0].positions[1].y);
	EXPECT_FLOAT_EQ(15.0f, shapes[0].positions[2].x);
	EXPECT_FLOAT_EQ(-17.5f, shapes[0].positions[2].y);
	EXPECT_FLOAT_EQ(21.875f, shapes[0].positions[3].x);
	EXPECT_FLOAT_EQ(-11.875f, shapes[0].positions[3].y);
	EXPECT_FLOAT_EQ(25.0f, shapes[0].positions[4].x);
	EXPECT_FLOAT_EQ(5.0f, shapes[0].positions[4].y);
}

TEST(CurvePath, PathQuadraticNoStartPoint)
{
	CurvePath path;
	path.QuadraticCurveTo(
		glm::vec2(15.0f, 26.0f),
		glm::vec2(33.4f, 19.2f),
		glm::vec2(-12.0f, 29.9f)
	);

	CurveSettings settings;
	settings.precision = 3;

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(3, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(-12.0f, shapes[0].positions[2].x);
	EXPECT_FLOAT_EQ(29.9f, shapes[0].positions[2].y);
}

TEST(CurvePath, PathQuadraticPrecisionHigh)
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

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(17, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(24.0f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(87.2f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(19.618299f, shapes[0].positions[6].x);
	EXPECT_FLOAT_EQ(71.68959f, shapes[0].positions[6].y);
	EXPECT_FLOAT_EQ(33.91f, shapes[0].positions[16].x);
	EXPECT_FLOAT_EQ(88.91f, shapes[0].positions[16].y);
}

TEST(CurvePath, PathQuadraticPrecisionOne)
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

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(2, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(57.36f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(14.25f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(63.57f, shapes[0].positions[1].x);
	EXPECT_FLOAT_EQ(49.886f, shapes[0].positions[1].y);
}

TEST(CurvePath, PathQuadraticPrecisionTwoCurves)
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

	std::vector<Polygon> shapes = path.ConvertToPolygons(settings);

	ASSERT_EQ(1, shapes.size());
	ASSERT_EQ(29, shapes[0].positions.size());
	EXPECT_FLOAT_EQ(222.8f, shapes[0].positions[0].x);
	EXPECT_FLOAT_EQ(34.9f, shapes[0].positions[0].y);
	EXPECT_FLOAT_EQ(88.8812f, shapes[0].positions[14].x);
	EXPECT_FLOAT_EQ(-12.9f, shapes[0].positions[14].y);
	EXPECT_FLOAT_EQ(16.87f, shapes[0].positions[28].x);
	EXPECT_FLOAT_EQ(912.08f, shapes[0].positions[28].y);
}