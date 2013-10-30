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
	Bounds bounds(
		glm::vec2(-887.8f, 92.9f),
		glm::vec2(88.81f, 215.9f)
	);

	EXPECT_VEC2_EQ(-887.8f, 92.9f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(88.81f, 215.9f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}

TEST(Bounds, ConstructSwappedX)
{
	Bounds bounds(
		glm::vec2(25.08f, -76.2f),
		glm::vec2(3.98f, 88.77f)
	);

	EXPECT_VEC2_EQ(3.98f, -76.2f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(25.08, 88.77f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}

TEST(Bounds, ConstructSwappedY)
{
	Bounds bounds(
		glm::vec2(10.78f, 107.82f),
		glm::vec2(45.78f, 15.8f)
	);

	EXPECT_VEC2_EQ(10.78f, 15.8f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(45.78f, 107.82f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}

TEST(Bounds, Copy)
{
	Bounds bounds(
		glm::vec2(-45.8f, 99.2f),
		glm::vec2(16.9f, 107.88f)
	);

	Bounds copy(bounds);

	EXPECT_VEC2_EQ(-45.8f, 99.2f, copy.GetMinimum());
	EXPECT_VEC2_EQ(16.9f, 107.88f, copy.GetMaximum());
}

TEST(Bounds, Assignment)
{
	Bounds bounds(
		glm::vec2(22.8f, 15.9f),
		glm::vec2(-117.8f, 67.83f)
	);

	Bounds copy = bounds;

	EXPECT_VEC2_EQ(-117.8f, 15.9f, copy.GetMinimum());
	EXPECT_VEC2_EQ(22.8f, 67.83f, copy.GetMaximum());
}