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

TEST(Bounds, SetWidth)
{
	Bounds bounds(
		glm::vec2(-16.97f, 11.98f),
		glm::vec2(11.98f, 112.8f)
	);

	bounds.SetWidth(116.9f);

	EXPECT_VEC2_EQ(
		-16.97f,
		11.98f,
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		-16.97f + 116.9f,
		112.8f,
		bounds.GetMaximum()
	);
}

TEST(Bounds, SetWidthNegative)
{
	Bounds bounds(
		glm::vec2(11.87f, 116.8f),
		glm::vec2(14.98f, 216.87f)
	);

	bounds.SetWidth(-197.8f);

	EXPECT_VEC2_EQ(
		11.87f - 197.8f,
		116.8f,
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		11.87f,
		216.87f,
		bounds.GetMaximum()
	);
}

TEST(Bounds, SetWidthInvalid)
{
	Bounds bounds;

	bounds.SetWidth(76.9f);

	EXPECT_FLOAT_EQ(0.0f, bounds.GetMinimum().x);
	EXPECT_FLOAT_EQ(76.9f, bounds.GetMaximum().x);
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

TEST(Bounds, SetHeight)
{
	Bounds bounds(
		glm::vec2(16.8f, 116.8f),
		glm::vec2(21.98f, 156.87f)
	);

	bounds.SetHeight(60.0f);

	EXPECT_VEC2_EQ(
		16.8f,
		116.8f,
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		21.98f,
		116.8f + 60.0f,
		bounds.GetMaximum()
	);
}

TEST(Bounds, SetHeightNegative)
{
	Bounds bounds(
		glm::vec2(-16.8f, 11.08f),
		glm::vec2(35.87f, 116.9f)
	);

	bounds.SetHeight(-118.7f);

	EXPECT_VEC2_EQ(
		-16.8f,
		11.08f - 118.7f,
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		35.87f,
		11.08f,
		bounds.GetMaximum()
	);
}

TEST(Bounds, SetHeightInvalid)
{
	Bounds bounds;

	bounds.SetHeight(1116.8f);

	EXPECT_FLOAT_EQ(0.0f, bounds.GetMinimum().y);
	EXPECT_FLOAT_EQ(1116.8f, bounds.GetMaximum().y);
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

TEST(Bounds, SetHalfWidth)
{
	Bounds bounds(
		glm::vec2(14.5f, 118.7f),
		glm::vec2(36.8f, 156.87f)
	);

	bounds.SetHalfWidth(15.5f);

	EXPECT_VEC2_EQ(
		14.5f + (36.8f - 14.5f) / 2.0f,
		118.7f + (156.87f - 118.7f) / 2.0f,
		bounds.GetCenter()
	);
	EXPECT_VEC2_EQ(
		14.5f + (36.8f - 14.5f) / 2.0f - 15.5f,
		118.7f, 
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		14.5f + (36.8f - 14.5f) / 2.0f + 15.5f,
		156.87f, 
		bounds.GetMaximum()
	);
}

TEST(Bounds, SetHalfWidthNegative)
{
	Bounds bounds(
		glm::vec2(11.87f, 87.86f),
		glm::vec2(11.97f, 1964.2f)
	);

	bounds.SetHalfWidth(-33.7f);

	EXPECT_VEC2_EQ(
		11.87f + (11.97f - 11.87f) / 2.0f,
		87.86f + (1964.2f - 87.86f) / 2.0f,
		bounds.GetCenter()
	);
	EXPECT_VEC2_EQ(
		11.87f + (11.97f - 11.87f) / 2.0f - 33.7f,
		87.86f,
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		11.87f + (11.97f - 11.87f) / 2.0f + 33.7f,
		1964.2f,
		bounds.GetMaximum()
	);
}

TEST(Bounds, SetHalfWidthInvalid)
{
	Bounds bounds;

	bounds.SetHalfWidth(33.8f);

	EXPECT_FLOAT_EQ(-33.8f, bounds.GetMinimum().x);
	EXPECT_FLOAT_EQ(33.8f, bounds.GetMaximum().x);
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

TEST(Bounds, SetHalfHeight)
{
	Bounds bounds(
		glm::vec2(-3.7f, 116.87f),
		glm::vec2(25.8f, 150.12f)
	);

	bounds.SetHalfHeight(44.8f);

	EXPECT_VEC2_EQ(
		-3.7f + (25.8f - -3.7f) / 2.0f,
		116.87f + (150.12f - 116.87f) / 2.0f,
		bounds.GetCenter()
	);
	EXPECT_VEC2_EQ(
		-3.7f,
		116.87f + (150.12f - 116.87f) / 2.0f - 44.8f, 
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		25.8f,
		116.87f + (150.12f - 116.87f) / 2.0f + 44.8f, 
		bounds.GetMaximum()
	);
}

TEST(Bounds, SetHalfHeightNegative)
{
	Bounds bounds(
		glm::vec2(1.87f, 116.87f),
		glm::vec2(16.43f, 198.78f)
	);

	bounds.SetHalfHeight(-3.57f);

	EXPECT_VEC2_EQ(
		1.87f + (16.43f - 1.87f) / 2.0f,
		116.87f + (198.78f - 116.87f) / 2.0f,
		bounds.GetCenter()
	);
	EXPECT_VEC2_EQ(
		1.87f,
		116.87f + (198.78f - 116.87f) / 2.0f - 3.57f,
		bounds.GetMinimum()
	);
	EXPECT_VEC2_EQ(
		16.43f,
		116.87f + (198.78f - 116.87f) / 2.0f + 3.57f,
		bounds.GetMaximum()
	);
}

TEST(Bounds, SetHalfHeightInvalid)
{
	Bounds bounds;

	bounds.SetHalfHeight(16.98f);

	EXPECT_FLOAT_EQ(-16.98f, bounds.GetMinimum().y);
	EXPECT_FLOAT_EQ(16.98f, bounds.GetMaximum().y);
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

TEST(Bounds, IsIntersected)
{
	Bounds bounds_left(
		glm::vec2(-25.0f, -25.0f),
		glm::vec2(50.0f, 50.0f)
	);

	Bounds bounds_right(
		glm::vec2(0.0f, 0.0f),
		glm::vec2(100.0f, 100.0f)
	);

	EXPECT_TRUE(bounds_left.IsIntersected(bounds_right));
}

TEST(Bounds, IsIntersectedOverlapBoth)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(90.0f, -150.0f),
		glm::vec2(190.0f, -90.0f)
	);

	EXPECT_TRUE(bounds_left.IsIntersected(bounds_right));
}

TEST(Bounds, IsIntersectedOverlapHorizontal)
{
	Bounds bounds_left(
		glm::vec2(25.0f, 75.0f),
		glm::vec2(100.0f, 150.0f)
	);

	Bounds bounds_right(
		glm::vec2(50.0f, 200.0f),
		glm::vec2(150.0f, 250.0f)
	);

	EXPECT_FALSE(bounds_left.IsIntersected(bounds_right));
}

TEST(Bounds, IsIntersectedOverlapVertical)
{
	Bounds bounds_left(
		glm::vec2(40.0f, 50.0f),
		glm::vec2(60.0f, 70.0f)
	);

	Bounds bounds_right(
		glm::vec2(80.0f, 55.0f),
		glm::vec2(120.0f, 120.0f)
	);

	EXPECT_FALSE(bounds_left.IsIntersected(bounds_right));
}

TEST(Bounds, IsIntersectedOutsideBoth)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(-250.0f, 101.0f),
		glm::vec2(-215.0f, 113.0f)
	);

	EXPECT_FALSE(bounds_left.IsIntersected(bounds_right));
}

TEST(Bounds, IsIntersectedOutsideHorizontal)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(-150.0f, -25.0f),
		glm::vec2(-101.0f, 25.0f)
	);

	EXPECT_FALSE(bounds_left.IsIntersected(bounds_right));
}

TEST(Bounds, IsIntersectedOutsideVertical)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(20.0f, 150.0f),
		glm::vec2(20.0f, 175.0f)
	);

	EXPECT_FALSE(bounds_left.IsIntersected(bounds_right));
}

TEST(Bounds, IsIntersectedInvalidLeft)
{
	Bounds bounds_left;

	Bounds bounds_right(
		glm::vec2(-33.6f, 55.8f),
		glm::vec2(117.9f, 168.8f)
	);

	EXPECT_FALSE(bounds_left.IsIntersected(bounds_right));
}

TEST(Bounds, IsIntersectedInvalidRight)
{
	Bounds bounds_left(
		glm::vec2(17.98f, 110.8f),
		glm::vec2(125.8f, 256.8f)
	);

	Bounds bounds_right;

	EXPECT_FALSE(bounds_left.IsIntersected(bounds_right));
}

TEST(Bounds, Intersect)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(-150.0f, -56.0f),
		glm::vec2(67.0f, 119.0f)
	);

	bounds_left.Intersect(bounds_right);

	EXPECT_VEC2_EQ(-100.0f, -56.0f, bounds_left.GetMinimum());
	EXPECT_VEC2_EQ(67.0f, 100.0f, bounds_left.GetMaximum());
}

TEST(Bounds, IntersectOutside)
{
	Bounds bounds_left(
		glm::vec2(35.81f, 12.9f),
		glm::vec2(157.9f, 115.0f)
	);

	Bounds bounds_right(
		glm::vec2(-156.8f, 2.8f),
		glm::vec2(-59.9f, 8.7f)
	);

	bounds_left.Intersect(bounds_right);

	EXPECT_VEC2_EQ(35.81f, 12.9f, bounds_left.GetMinimum());
	EXPECT_VEC2_EQ(157.9f, 115.0f, bounds_left.GetMaximum());
}

TEST(Bounds, GetIntersection)
{
	Bounds bounds_left(
		glm::vec2(456.8f, 12.9f),
		glm::vec2(1078.2f, 115.8f)
	);

	Bounds bounds_right(
		glm::vec2(115.8f, 13.8f),
		glm::vec2(667.9f, 87.8f)
	);

	Bounds intersection = bounds_left.GetIntersection(bounds_right);

	EXPECT_VEC2_EQ(456.8f, 13.8f, intersection.GetMinimum());
	EXPECT_VEC2_EQ(667.9f, 87.8f, intersection.GetMaximum());
}

TEST(Bounds, GetIntersectionOutside)
{
	Bounds bounds_left(
		glm::vec2(55.0f, 12.8f),
		glm::vec2(314.9f, 45.9f)
	);

	Bounds bounds_right(
		glm::vec2(608.0f, 156.9f),
		glm::vec2(898.2f, 278.2f)
	);

	Bounds intersection = bounds_left.GetIntersection(bounds_right);

	EXPECT_FALSE(intersection.IsValid());
}

TEST(Bounds, IsContained)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(25.0f, 25.0f),
		glm::vec2(50.0f, 50.0f)
	);

	EXPECT_TRUE(bounds_left.IsContained(bounds_right));
}

TEST(Bounds, IsContainedOverlapBoth)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(55.0f, 78.0f),
		glm::vec2(225.0f, 178.0f)
	);

	EXPECT_FALSE(bounds_left.IsContained(bounds_right));
}

TEST(Bounds, IsContainedOverlapHorizontal)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(75.0f, -40.0f),
		glm::vec2(125.0f, 30.0f)
	);

	EXPECT_FALSE(bounds_left.IsContained(bounds_right));
}

TEST(Bounds, IsContainedOverlapVertical)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(33.0f, -200.0f),
		glm::vec2(34.0f, -80.0f)
	);

	EXPECT_FALSE(bounds_left.IsContained(bounds_right));
}

TEST(Bounds, IsContainedOutsideBoth)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(777.0f, 777.0f),
		glm::vec2(888.0f, 888.0f)
	);

	EXPECT_FALSE(bounds_left.IsContained(bounds_right));
}

TEST(Bounds, IsContainedOutsideHorizontal)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(150.0f, 15.0f),
		glm::vec2(250.0f, 66.0f)
	);

	EXPECT_FALSE(bounds_left.IsContained(bounds_right));
}

TEST(Bounds, IsContainedOutsideVertical)
{
	Bounds bounds_left(
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);

	Bounds bounds_right(
		glm::vec2(-56.0f, -2000.0f),
		glm::vec2(-43.0f, -1500.0f)
	);

	EXPECT_FALSE(bounds_left.IsContained(bounds_right));
}

TEST(Bounds, IsContainedInvalidLeft)
{
	Bounds bounds_left;

	Bounds bounds_right(
		glm::vec2(55.8f, 112.8f),
		glm::vec2(112.8f, 187.12f)
	);

	EXPECT_FALSE(bounds_left.IsContained(bounds_right));
}

TEST(Bounds, IsContainedInvalidRight)
{
	Bounds bounds_left(
		glm::vec2(-15.7f, 12.8f),
		glm::vec2(25.8f, 25.8f)
		);

	Bounds bounds_right;

	EXPECT_FALSE(bounds_left.IsContained(bounds_right));
}

TEST(Bounds, Unite)
{
	Bounds left(
		glm::vec2(15.9f, 22.5f),
		glm::vec2(16.87f, 23.8f)
	);

	Bounds right(
		glm::vec2(33.8f, 82.2f),
		glm::vec2(67.2f, 119.8f)
	);

	left.Unite(right);

	EXPECT_VEC2_EQ(15.9f, 22.5f, left.GetMinimum());
	EXPECT_VEC2_EQ(67.2f, 119.8f, left.GetMaximum());
}

TEST(Bounds, UniteInvalidLeft)
{
	Bounds left;

	Bounds right(
		glm::vec2(-17.98f, 11.8f),
		glm::vec2(15.82f, 188.2f)
	);

	left.Unite(right);

	EXPECT_FALSE(left.IsValid());
}

TEST(Bounds, UniteInvalidRight)
{
	Bounds left(
		glm::vec2(22.98f, 15.87f),
		glm::vec2(56.78f, 34.98f)
	);

	Bounds right;

	left.Unite(right);

	EXPECT_TRUE(left.IsValid());
	EXPECT_VEC2_EQ(22.98f, 15.87f, left.GetMinimum());
	EXPECT_VEC2_EQ(56.78f, 34.98f, left.GetMaximum());
}

TEST(Bounds, GetUnited)
{
	Bounds left(
		glm::vec2(3.8f, 11.8f),
		glm::vec2(56.9f, 16.87f)
	);

	Bounds right(
		glm::vec2(9.87f, 119.14f),
		glm::vec2(11.8f, 1176.72f)
	);

	Bounds united = left.GetUnited(right);

	EXPECT_VEC2_EQ(3.8f, 11.8f, united.GetMinimum());
	EXPECT_VEC2_EQ(56.9f, 1176.72f, united.GetMaximum());
}

TEST(Bounds, GetUnitedInvalidLeft)
{
	Bounds left;

	Bounds right(
		glm::vec2(15.87f, 119.8f),
		glm::vec2(2.876f, 18.76f)
	);

	Bounds united = left.GetUnited(right);

	EXPECT_FALSE(united.IsValid());
}

TEST(Bounds, GetUnitedInvalidRight)
{
	Bounds left(
		glm::vec2(22.87f, 112.7f),
		glm::vec2(35.97f, 176.66f)
	);

	Bounds right;

	Bounds united = left.GetUnited(right);

	EXPECT_FALSE(united.IsValid());
}