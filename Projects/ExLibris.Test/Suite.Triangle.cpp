#include "ExLibris.Test.PCH.h"

#include "Triangle.h"

using namespace ExLibris;

TEST(Triangle, Construct)
{
	glm::vec2 a(15.9f, 99.2f);
	glm::vec2 b(-83.2f, 71.8f);
	glm::vec2 c(87.2f, -77.1f);

	Triangle triangle(a, b, c);

	EXPECT_FLOAT_EQ(15.9f, triangle.a.x);
	EXPECT_FLOAT_EQ(99.2f, triangle.a.y);
	EXPECT_FLOAT_EQ(-83.2f, triangle.b.x);
	EXPECT_FLOAT_EQ(71.8f, triangle.b.y);
	EXPECT_FLOAT_EQ(87.2f, triangle.c.x);
	EXPECT_FLOAT_EQ(-77.1f, triangle.c.y);
}

TEST(Triangle, IsConvex)
{
	glm::vec2 a(0.0f, 0.0f);
	glm::vec2 b(100.0f, 100.0f);
	glm::vec2 c(0.0f, 100.0f);

	Triangle triangle(a, b, c);

	EXPECT_TRUE(triangle.IsConvex());
}

TEST(Triangle, IsNotConvex)
{
	glm::vec2 a(0.0f, 0.0f);
	glm::vec2 b(0.0f, 100.0f);
	glm::vec2 c(100.0f, 100.0f);

	Triangle triangle(a, b, c);

	EXPECT_FALSE(triangle.IsConvex());
}

TEST(Triangle, IsPositionInside)
{
	glm::vec2 a(0.0f, 0.0f);
	glm::vec2 b(100.0f, 100.0f);
	glm::vec2 c(0.0f, 100.0f);

	Triangle triangle(a, b, c);

	EXPECT_TRUE(triangle.IsPositionInside(glm::vec2(50.0f, 50.0f)));
}

TEST(Triangle, IsPositionOutsideLeft)
{
	glm::vec2 a(0.0f, 0.0f);
	glm::vec2 b(100.0f, 100.0f);
	glm::vec2 c(0.0f, 100.0f);

	Triangle triangle(a, b, c);

	EXPECT_FALSE(triangle.IsPositionInside(glm::vec2(-50.0f, 50.0f)));
}

TEST(Triangle, IsPositionOutsideRight)
{
	glm::vec2 a(0.0f, 0.0f);
	glm::vec2 b(100.0f, 100.0f);
	glm::vec2 c(0.0f, 100.0f);

	Triangle triangle(a, b, c);

	EXPECT_FALSE(triangle.IsPositionInside(glm::vec2(150.0f, 50.0f)));
}

TEST(Triangle, IsPositionOutsideUp)
{
	glm::vec2 a(0.0f, 0.0f);
	glm::vec2 b(100.0f, 100.0f);
	glm::vec2 c(0.0f, 100.0f);

	Triangle triangle(a, b, c);

	EXPECT_FALSE(triangle.IsPositionInside(glm::vec2(50.0f, -50.0f)));
}

TEST(Triangle, IsPositionOutsideDown)
{
	glm::vec2 a(0.0f, 0.0f);
	glm::vec2 b(100.0f, 100.0f);
	glm::vec2 c(0.0f, 100.0f);

	Triangle triangle(a, b, c);

	EXPECT_FALSE(triangle.IsPositionInside(glm::vec2(50.0f, 150.0f)));
}