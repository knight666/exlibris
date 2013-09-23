#include "ExLibris.Test.PCH.h"

#include "Line.h"

using namespace ExLibris;

TEST(Line, Construct)
{
	Line line(
		glm::vec2(10.0f, 66.92f),
		glm::vec2(-7.92f, 90.12f)
	);

	EXPECT_VEC2_EQ(10.0f, 66.92f, line.start);
	EXPECT_VEC2_EQ(-7.92f, 90.12f, line.end);
}

TEST(Line, QuadHorizontal)
{
	Line line(
		glm::vec2(0.0f, 0.0f),
		glm::vec2(100.0f, 0.0f)
	);

	Quad quad = line.ConstructQuad(20.0f);

	EXPECT_VEC2_EQ(0.0f, -10.0f, quad.ul);
	EXPECT_VEC2_EQ(0.0f, 10.0f, quad.ll);
	EXPECT_VEC2_EQ(100.0f, -10.0f, quad.ur);
	EXPECT_VEC2_EQ(100.0f, 10.0f, quad.lr);
}

TEST(Line, QuadHorizontalBackwards)
{
	Line line(
		glm::vec2(250.0f, 50.0f),
		glm::vec2(150.0f, 50.0f)
	);

	Quad quad = line.ConstructQuad(10.0f);

	EXPECT_VEC2_EQ(150.0f, 45.0f, quad.ul);
	EXPECT_VEC2_EQ(150.0f, 55.0f, quad.ll);
	EXPECT_VEC2_EQ(250.0f, 45.0f, quad.ur);
	EXPECT_VEC2_EQ(250.0f, 55.0f, quad.lr);
}