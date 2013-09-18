#include "ExLibris.Test.PCH.h"

#include "PolygonShape.h"

using namespace ExLibris;

TEST(PolygonShape, TriangulateEmpty)
{
	PolygonShape polygon;

	TriangleList* list = polygon.Triangulate();

	EXPECT_EQ(nullptr, list->positions);
	EXPECT_EQ(0, list->vertex_count);
}

TEST(PolygonShape, TriangulateOne)
{
	PolygonShape polygon;

	Polygon shape;
	shape.positions.push_back(glm::vec2(56.9f, -91.9f));
	shape.positions.push_back(glm::vec2(6.43f, 12.8f));
	shape.positions.push_back(glm::vec2(-11.9f, 14.5f));
	
	polygon.AddShape(shape);

	TriangleList* list = polygon.Triangulate();

	ASSERT_NE(nullptr, list->positions);
	ASSERT_EQ(3, list->vertex_count);
	EXPECT_FLOAT_EQ(6.43f, list->positions[0].x);
	EXPECT_FLOAT_EQ(12.8f, list->positions[0].y);
	EXPECT_FLOAT_EQ(-11.9f, list->positions[1].x);
	EXPECT_FLOAT_EQ(14.5f, list->positions[1].y);
	EXPECT_FLOAT_EQ(56.9f, list->positions[2].x);
	EXPECT_FLOAT_EQ(-91.9f, list->positions[2].y);
}

TEST(PolygonShape, TriangulateTwo)
{
	PolygonShape polygon;

	Polygon shape;
	shape.positions.push_back(glm::vec2(16.4f, 78.8f));
	shape.positions.push_back(glm::vec2(12.9f, 105.9f));
	shape.positions.push_back(glm::vec2(-25.9f, 56.0f));
	shape.positions.push_back(glm::vec2(-125.0f, 32.9f));

	polygon.AddShape(shape);

	TriangleList* list = polygon.Triangulate();

	ASSERT_NE(nullptr, list->positions);
	ASSERT_EQ(12, list->vertex_count);
	EXPECT_FLOAT_EQ(-25.9f, list->positions[0].x);
	EXPECT_FLOAT_EQ(56.0f, list->positions[0].y);
	EXPECT_FLOAT_EQ(-125.0f, list->positions[1].x);
	EXPECT_FLOAT_EQ(32.9f, list->positions[1].y);
	EXPECT_FLOAT_EQ(58.82f, list->positions[2].x);
	EXPECT_FLOAT_EQ(11.0f, list->positions[2].y);
	EXPECT_FLOAT_EQ(16.4f, list->positions[3].x);
	EXPECT_FLOAT_EQ(78.8f, list->positions[3].y);
	EXPECT_FLOAT_EQ(-25.9f, list->positions[4].x);
	EXPECT_FLOAT_EQ(56.0f, list->positions[4].y);
	EXPECT_FLOAT_EQ(58.82f, list->positions[5].x);
	EXPECT_FLOAT_EQ(11.0f, list->positions[5].y);
}

TEST(PolygonShape, TriangulateTwoPolygons)
{
	PolygonShape shape;

	Polygon polygon_first;
	polygon_first.positions.push_back(glm::vec2(-20.0f, -10.0f));
	polygon_first.positions.push_back(glm::vec2(30.0f, 10.0f));
	polygon_first.positions.push_back(glm::vec2(30.0f, 55.0f));

	Polygon polygon_second;
	polygon_second.positions.push_back(glm::vec2(50.0f, -20.0f));
	polygon_second.positions.push_back(glm::vec2(80.0f, 10.0f));
	polygon_second.positions.push_back(glm::vec2(120.0f, 20.0f));

	shape.AddShape(polygon_first);
	shape.AddShape(polygon_second);

	TriangleList* list = shape.Triangulate();

	ASSERT_NE(nullptr, list->positions);
	ASSERT_EQ(18, list->vertex_count);
	EXPECT_FLOAT_EQ(-20.0f, list->positions[0].x);
	EXPECT_FLOAT_EQ(-10.0f, list->positions[0].y);
	EXPECT_FLOAT_EQ(30.0f, list->positions[1].x);
	EXPECT_FLOAT_EQ(10.0f, list->positions[1].y);
	EXPECT_FLOAT_EQ(30.0f, list->positions[2].x);
	EXPECT_FLOAT_EQ(55.0f, list->positions[2].y);
	EXPECT_FLOAT_EQ(50.0f, list->positions[3].x);
	EXPECT_FLOAT_EQ(-20.0f, list->positions[3].y);
	EXPECT_FLOAT_EQ(80.0f, list->positions[4].x);
	EXPECT_FLOAT_EQ(10.0f, list->positions[4].y);
	EXPECT_FLOAT_EQ(120.0f, list->positions[5].x);
	EXPECT_FLOAT_EQ(20.0f, list->positions[5].y);
}

TEST(PolygonShape, TriangulateTwoPolygonsHole)
{
	// o ------------- o
	// |               |
	// |       o       |
	// |     /   \     |
	// |    /     \    |
	// |   o ----- o   |
	// |               |
	// o --------------o

	PolygonShape shape;

	Polygon polygon_first;
	polygon_first.positions.push_back(glm::vec2(-50.0f, -50.0f));
	polygon_first.positions.push_back(glm::vec2(50.0f, -50.0f));
	polygon_first.positions.push_back(glm::vec2(50.0f, 50.0f));
	polygon_first.positions.push_back(glm::vec2(-50.0f, 50.0f));

	Polygon polygon_second;
	polygon_second.positions.push_back(glm::vec2(0.0f, -25.0f));
	polygon_second.positions.push_back(glm::vec2(-25.0f, 25.0f));
	polygon_second.positions.push_back(glm::vec2(25.0f, 25.0f));

	shape.AddShape(polygon_first);
	shape.AddShape(polygon_second);

	TriangleList* list = shape.Triangulate();

	ASSERT_NE(nullptr, list->positions);
	ASSERT_EQ(21, list->vertex_count);
	
	EXPECT_FLOAT_EQ(-50.0f, list->positions[0].x);
	EXPECT_FLOAT_EQ(50.0f, list->positions[0].y);
	EXPECT_FLOAT_EQ(-25.0f, list->positions[1].x);
	EXPECT_FLOAT_EQ(25.0f, list->positions[1].y);
	EXPECT_FLOAT_EQ(50.0f, list->positions[2].x);
	EXPECT_FLOAT_EQ(50.0f, list->positions[2].y);

	EXPECT_FLOAT_EQ(-25.0f, list->positions[18].x);
	EXPECT_FLOAT_EQ(25.0f, list->positions[18].y);
	EXPECT_FLOAT_EQ(25.0f, list->positions[19].x);
	EXPECT_FLOAT_EQ(25.0f, list->positions[19].y);
	EXPECT_FLOAT_EQ(50.0f, list->positions[20].x);
	EXPECT_FLOAT_EQ(50.0f, list->positions[20].y);
}