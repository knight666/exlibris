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

TEST(Bounds, CopyInvalid)
{
	Bounds bounds;

	Bounds copy(bounds);

	EXPECT_FALSE(copy.IsValid());
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

TEST(Bounds, AssignmentInvalid)
{
	Bounds bounds;

	Bounds copy = bounds;

	EXPECT_FALSE(copy.IsValid());
}

TEST(Bounds, SetMinimum)
{
	Bounds bounds(
		glm::vec2(-98.8f, 99.8f),
		glm::vec2(12.8f, 812.9f)
	);

	bounds.SetMinimum(glm::vec2(-107.2f, 8.87f));

	EXPECT_VEC2_EQ(-107.2f, 8.87f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(12.8f, 812.9f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}

TEST(Bounds, SetMinimumSwappedX)
{
	Bounds bounds(
		glm::vec2(23.87f, 1.87f),
		glm::vec2(87.7f, 117.8f)
	);

	bounds.SetMinimum(glm::vec2(187.9f, -8.8f));

	EXPECT_VEC2_EQ(87.7f, -8.8f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(187.9f, 117.8f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}

TEST(Bounds, SetMinimumSwappedY)
{
	Bounds bounds(
		glm::vec2(1.87f, -98.6f),
		glm::vec2(38.12f, 8.76f)
	);

	bounds.SetMinimum(glm::vec2(-98.76f, 115.8f));

	EXPECT_VEC2_EQ(-98.76f, 8.76f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(38.12f, 115.8f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}

TEST(Bounds, SetMinimumInvalid)
{
	Bounds bounds;

	bounds.SetMinimum(glm::vec2(32.57f, 19.97f));

	EXPECT_VEC2_EQ(32.57f, 19.97f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(32.57f, 19.97f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}

TEST(Bounds, SetMaximum)
{
	Bounds bounds(
		glm::vec2(3.87f, 18.97f),
		glm::vec2(33.87f, 188.81f)
	);

	bounds.SetMaximum(glm::vec2(42.0f, 230.17f));

	EXPECT_VEC2_EQ(3.87f, 18.97f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(42.0f, 230.17f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}

TEST(Bounds, SetMaximumSwappedX)
{
	Bounds bounds(
		glm::vec2(16.7f, 32.8f),
		glm::vec2(110.8f, 881.8f)
	);

	bounds.SetMaximum(glm::vec2(-76.8f, 1085.7f));

	EXPECT_VEC2_EQ(-76.8f, 32.8f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(16.7f, 1085.7f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}

TEST(Bounds, SetMaximumSwappedY)
{
	Bounds bounds(
		glm::vec2(18.77f, 15.87f),
		glm::vec2(-86.8f, 110.8f)
	);

	bounds.SetMaximum(glm::vec2(116.9f, -78.8f));

	EXPECT_VEC2_EQ(-86.8f, -78.8f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(116.9f, 15.87f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}

TEST(Bounds, SetMaximumInvalid)
{
	Bounds bounds;

	bounds.SetMaximum(glm::vec2(16.87f, 12.787f));

	EXPECT_VEC2_EQ(16.87f, 12.787f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(16.87f, 12.787f, bounds.GetMaximum());
	EXPECT_TRUE(bounds.IsValid());
}

TEST(Bounds, GetDimensions)
{
	Bounds bounds(
		glm::vec2(17.98f, -99.12f),
		glm::vec2(108.72f, 15.8f)
	);

	EXPECT_VEC2_EQ(
		(108.72f - 17.98f),
		(15.8f - -99.12f),
		bounds.GetDimensions()
	);
}

TEST(Bounds, GetDimensionsInvalid)
{
	Bounds bounds;

	EXPECT_VEC2_EQ(0.0f, 0.0f, bounds.GetDimensions());
}

TEST(Bounds, SetDimensions)
{
	Bounds bounds(
		glm::vec2(12.97f, 19.7f),
		glm::vec2(188.97f, 81.8f)
	);

	bounds.SetDimensions(glm::vec2(53.0f, 53.0f));

	EXPECT_VEC2_EQ(12.97f, 19.7f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(12.97f + 53.0f, 19.7f + 53.0f, bounds.GetMaximum());
}

TEST(Bounds, SetDimensionsNegativeWidth)
{
	Bounds bounds(
		glm::vec2(-15.86f, 17.98f),
		glm::vec2(33.76f, 35.97f)
	);

	bounds.SetDimensions(glm::vec2(-10.0f, 22.8f));

	EXPECT_VEC2_EQ(-15.86f - 10.0f, 17.98f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(-15.86f, 17.98f + 22.8f, bounds.GetMaximum());
}

TEST(Bounds, SetDimensionsNegativeHeight)
{
	Bounds bounds(
		glm::vec2(32.45f, 117.98f),
		glm::vec2(45.87f, 256.16f)
	);

	bounds.SetDimensions(glm::vec2(33.87f, -109.87f));

	EXPECT_VEC2_EQ(32.45f, 117.98f - 109.87f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(32.45f + 33.87f, 117.98f, bounds.GetMaximum());
}

TEST(Bounds, SetDimensionsInvalid)
{
	Bounds bounds;

	bounds.SetDimensions(glm::vec2(19.9f, 112.9f));

	EXPECT_VEC2_EQ(0.0f, 0.0f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(19.9f, 112.9f, bounds.GetMaximum());
}

TEST(Bounds, GetWidth)
{
	Bounds bounds(
		glm::vec2(15.89f, -5.8f),
		glm::vec2(115.8f, 881.98f)
	);

	EXPECT_FLOAT_EQ((115.8f - 15.89f), bounds.GetWidth());
}

TEST(Bounds, GetWidthInvalid)
{
	Bounds bounds;

	EXPECT_FLOAT_EQ(0.0f, bounds.GetWidth());
}

TEST(Bounds, GetHeight)
{
	Bounds bounds(
		glm::vec2(22.9f, -87.7f),
		glm::vec2(18.17f, 9.8765f)
	);

	EXPECT_FLOAT_EQ((87.7f + 9.8765f), bounds.GetHeight());
}

TEST(Bounds, GetHeightInvalid)
{
	Bounds bounds;

	EXPECT_FLOAT_EQ(0.0f, bounds.GetHeight());
}

TEST(Bounds, GetHalfDimensions)
{
	Bounds bounds(
		glm::vec2(6.9f, 12.78f),
		glm::vec2(16.87f, 29.9f)
	);

	EXPECT_VEC2_EQ(
		(16.87f - 6.9f) / 2.0f,
		(29.9f - 12.78f) / 2.0f,
		bounds.GetHalfDimensions()
	);
}

TEST(Bounds, GetHalfDimensionsInvalid)
{
	Bounds bounds;

	EXPECT_VEC2_EQ(0.0f, 0.0f, bounds.GetHalfDimensions());
}

TEST(Bounds, SetHalfDimensions)
{
	Bounds bounds(
		glm::vec2(10.0f, 10.0f),
		glm::vec2(50.0f, 50.0f)
	);

	bounds.SetHalfDimensions(glm::vec2(25.0f, 50.0f));

	EXPECT_VEC2_EQ(
		10.0f + (50.0f - 10.0f) / 2.0f,
		10.0f + (50.0f - 10.0f) / 2.0f,
		bounds.GetCenter()
	);
	EXPECT_VEC2_EQ(
		10.0f + (50.0f - 10.0f) / 2.0f - 25.0f,
		10.0f + (50.0f - 10.0f) / 2.0f - 50.0f, 
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		10.0f + (50.0f - 10.0f) / 2.0f + 25.0f,
		10.0f + (50.0f - 10.0f) / 2.0f + 50.0f, 
		bounds.GetMaximum()
	);
}

TEST(Bounds, SetHalfDimensionsNegativeWidth)
{
	Bounds bounds(
		glm::vec2(30.0f, 25.0f),
		glm::vec2(60.0f, 150.0f)
	);

	bounds.SetHalfDimensions(glm::vec2(-16.0f, 36.0f));

	EXPECT_VEC2_EQ(
		30.0f + (60.0f - 30.0f) / 2.0f,
		25.0f + (150.0f - 25.0f) / 2.0f,
		bounds.GetCenter()
	);
	EXPECT_VEC2_EQ(
		30.0f + (60.0f - 30.0f) / 2.0f - 16.0f,
		25.0f + (150.0f - 25.0f) / 2.0f - 36.0f, 
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		30.0f + (60.0f - 30.0f) / 2.0f + 16.0f,
		25.0f + (150.0f - 25.0f) / 2.0f + 36.0f, 
		bounds.GetMaximum()
	);
}

TEST(Bounds, SetHalfDimensionsNegativeHeight)
{
	Bounds bounds(
		glm::vec2(16.87f, 11.87f),
		glm::vec2(194.5f, 68.8f)
	);

	bounds.SetHalfDimensions(glm::vec2(16.78f, -1.987f));

	EXPECT_VEC2_EQ(
		16.87f + (194.5f - 16.87f) / 2.0f,
		11.87f + (68.8f - 11.87f) / 2.0f,
		bounds.GetCenter()
	);
	EXPECT_VEC2_EQ(
		16.87f + (194.5f - 16.87f) / 2.0f - 16.78f,
		11.87f + (68.8f - 11.87f) / 2.0f - 1.987f, 
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		16.87f + (194.5f - 16.87f) / 2.0f + 16.78f,
		11.87f + (68.8f - 11.87f) / 2.0f + 1.987f, 
		bounds.GetMaximum()
	);
}

TEST(Bounds, SetHalfDimensionsInvalid)
{
	Bounds bounds;

	bounds.SetHalfDimensions(glm::vec2(17.98, 22.83f));

	EXPECT_VEC2_EQ(
		0.0f,
		0.0f,
		bounds.GetCenter()
	);
	EXPECT_VEC2_EQ(
		-17.98f,
		-22.83f,
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		17.98f,
		22.83f,
		bounds.GetMaximum()
	);
}

TEST(Bounds, GetHalfWidth)
{
	Bounds bounds(
		glm::vec2(16.87f, 115.8f),
		glm::vec2(-38.8f, 108.9f)
	);

	EXPECT_FLOAT_EQ((38.8f + 16.87f) / 2.0f, bounds.GetHalfWidth());
}

TEST(Bounds, GetHalfWidthInvalid)
{
	Bounds bounds;

	EXPECT_FLOAT_EQ(0.0f, bounds.GetHalfWidth());
}

TEST(Bounds, GetHalfHeight)
{
	Bounds bounds(
		glm::vec2(33.9f, 88.8f),
		glm::vec2(16.87f, 18.8f)
	);

	EXPECT_FLOAT_EQ((88.8f - 18.8f) / 2.0f, bounds.GetHalfHeight());
}

TEST(Bounds, GetHalfHeightInvalid)
{
	Bounds bounds;

	EXPECT_FLOAT_EQ(0.0f, bounds.GetHalfHeight());
}

TEST(Bounds, GetCenter)
{
	Bounds bounds(
		glm::vec2(22.87f, 25.87f),
		glm::vec2(45.98f, 100.9f)
	);

	EXPECT_VEC2_EQ(
		22.87f + (45.98f - 22.87f) / 2.0f,
		25.87f + (100.9f - 25.87f) / 2.0f,
		bounds.GetCenter()
	);
}

TEST(Bounds, GetCenterInvalid)
{
	Bounds bounds;

	EXPECT_VEC2_EQ(0.0f, 0.0f, bounds.GetCenter());
}

TEST(Bounds, SetCenter)
{
	Bounds bounds(
		glm::vec2(100.0f, 100.0f),
		glm::vec2(250.0f, 250.0f)
	);

	bounds.SetCenter(glm::vec2(10.0f, 20.0f));

	EXPECT_VEC2_EQ(-65.0f, -55.0f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(85.0f, 95.0f, bounds.GetMaximum());
}

TEST(Bounds, SetCenterInvalid)
{
	Bounds bounds;

	bounds.SetCenter(glm::vec2(56.4f, -69.8f));

	EXPECT_VEC2_EQ(56.4f, -69.8f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(56.4f, -69.8f, bounds.GetMaximum());
}