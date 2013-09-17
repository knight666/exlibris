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

	Shape shape;
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

	Shape shape;
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