#include "ExLibris.Test.PCH.h"

#include <Bounds.h>

using namespace ExLibris;

TEST(Bounds, Construct)
{
	Bounds bounds;

	EXPECT_FALSE(bounds.IsValid());
}

TEST(Bounds, ConstructPositions)
{
	glm::vec2 position_min(-887.8f, 92.9f);
	glm::vec2 position_max(88.81f, 215.9f);

	Bounds bounds(position_min, position_max);

	EXPECT_VEC2_EQ(-887.8f, 92.9f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(88.81f, 215.9f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}