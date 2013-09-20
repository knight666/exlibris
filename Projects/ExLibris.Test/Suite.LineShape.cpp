#include "ExLibris.Test.PCH.h"

#include "LineShape.h"

using namespace ExLibris;

TEST(LineShape, Construct)
{
	LineShape shape;

	EXPECT_EQ(0, shape.GetPolygonCount());
}

TEST(LineShape, AddPolygon)
{
	LineShape shape;

	Polygon p;
	shape.AddPolygon(p);

	EXPECT_EQ(1, shape.GetPolygonCount());
}

TEST(LineShape, ClearPolygons)
{
	LineShape shape;

	Polygon p;
	shape.AddPolygon(p);

	shape.Clear();

	EXPECT_EQ(0, shape.GetPolygonCount());
}

TEST(LineShape, TriangulateStraightLine)
{
	LineShape shape;

	Polygon p;
	p += glm::vec2(-50.0f, 25.0f);
	p += glm::vec2(150.0f, 25.0f);

	shape.AddPolygon(p);

	TriangleList* triangles = shape.Triangulate(2.0f);
	ASSERT_EQ(6, triangles->vertex_count);

	EXPECT_FLOAT_EQ(150.0f, triangles->positions[0].x);
	EXPECT_FLOAT_EQ(24.0f, triangles->positions[0].y);
	EXPECT_FLOAT_EQ(-50.0f, triangles->positions[1].x);
	EXPECT_FLOAT_EQ(24.0f, triangles->positions[1].y);
	EXPECT_FLOAT_EQ(-50.0f, triangles->positions[2].x);
	EXPECT_FLOAT_EQ(26.0f, triangles->positions[2].y);

	EXPECT_FLOAT_EQ(150.0f, triangles->positions[3].x);
	EXPECT_FLOAT_EQ(24.0f, triangles->positions[3].y);
	EXPECT_FLOAT_EQ(-50.0f, triangles->positions[4].x);
	EXPECT_FLOAT_EQ(26.0f, triangles->positions[4].y);
	EXPECT_FLOAT_EQ(150.0f, triangles->positions[5].x);
	EXPECT_FLOAT_EQ(26.0f, triangles->positions[5].y);
}

TEST(LineShape, TriangulateCornerUp)
{
	LineShape shape;

	Polygon p;
	p += glm::vec2(0.0f, 100.0f);
	p += glm::vec2(50.0f, 100.0f);
	p += glm::vec2(50.0f, 50.0f);

	shape.AddPolygon(p);

	TriangleList* triangles = shape.Triangulate(10.0f);
	ASSERT_EQ(12, triangles->vertex_count);

	EXPECT_FLOAT_EQ(0.0f, triangles->positions[1].x);
	EXPECT_FLOAT_EQ(95.0f, triangles->positions[1].y);
	EXPECT_FLOAT_EQ(50.0f, triangles->positions[0].x);
	EXPECT_FLOAT_EQ(95.0f, triangles->positions[0].y);
	EXPECT_FLOAT_EQ(0.0f, triangles->positions[2].x);
	EXPECT_FLOAT_EQ(105.0f, triangles->positions[2].y);
	EXPECT_FLOAT_EQ(50.0f, triangles->positions[5].x);
	EXPECT_FLOAT_EQ(105.0f, triangles->positions[5].y);

	EXPECT_FLOAT_EQ(45.0f, triangles->positions[7].x);
	EXPECT_FLOAT_EQ(100.0f, triangles->positions[7].y);
	EXPECT_FLOAT_EQ(55.0f, triangles->positions[8].x);
	EXPECT_FLOAT_EQ(100.0f, triangles->positions[8].y);
	EXPECT_FLOAT_EQ(45.0f, triangles->positions[6].x);
	EXPECT_FLOAT_EQ(50.0f, triangles->positions[6].y);
	EXPECT_FLOAT_EQ(55.0f, triangles->positions[11].x);
	EXPECT_FLOAT_EQ(50.0, triangles->positions[11].y);
}