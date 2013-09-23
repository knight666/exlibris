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