#include "ExLibris.Test.PCH.h"

#include "Polygon.h"

using namespace ExLibris;

TEST(Polygon, AddPosition)
{
	Polygon p;

	p += glm::vec2(25.3f, -92.0f);

	EXPECT_EQ(1, p.positions.size());
	EXPECT_FLOAT_EQ(25.3f, p.positions[0].x);
	EXPECT_FLOAT_EQ(-92.0f, p.positions[0].y);
}

TEST(Polygon, IsPositionInside)
{
	Polygon p;

	p += glm::vec2(-10.0f, -10.0f);
	p += glm::vec2(10.0f, -10.0f);
	p += glm::vec2(10.0f, 10.0f);
	p += glm::vec2(-10.0f, 10.0f);

	EXPECT_TRUE(p.IsPositionInside(glm::vec2(5.0f, 5.0f)));
}

TEST(Polygon, IsPositionInsideConcave)
{
	Polygon p;

	// o -- o     o -- o
	// |     \   /     |
	// |      \ /      |
	// |       o       |
	// |               |
	// |               |
	// o ------------- o

	p += glm::vec2(-10.0f, -10.0f);
	p += glm::vec2( -8.0f, -10.0f);
	p += glm::vec2(  0.0f,  -5.0f);
	p += glm::vec2(  8.0f, -10.0f);
	p += glm::vec2( 10.0f, -10.0f);
	p += glm::vec2( 10.0f,  10.0f);
	p += glm::vec2(-10.0f,  10.0f);

	EXPECT_TRUE(p.IsPositionInside(glm::vec2(-9.0f, -8.0f)));
	EXPECT_FALSE(p.IsPositionInside(glm::vec2( 0.0f, -8.0f)));
	EXPECT_TRUE(p.IsPositionInside(glm::vec2( 9.0f, -8.0f)));
}

TEST(Polygon, IsPositionInsideEmpty)
{
	Polygon p;

	EXPECT_FALSE(p.IsPositionInside(glm::vec2(7.9f, -67.0f)));
}

TEST(Polygon, IsPositionInsideNotEnoughVertices)
{
	Polygon p;

	p += glm::vec2(15.0f, 99.8f);
	p += glm::vec2(33.5f, 91.7f);

	EXPECT_FALSE(p.IsPositionInside(glm::vec2(14.0f, 12.0f)));
}

TEST(Polygon, Intersects)
{
	Polygon p_left;
	p_left += glm::vec2(-10.0f, -10.0f);
	p_left += glm::vec2(10.0f, -10.0f);
	p_left += glm::vec2(10.0f, 10.0f);
	p_left += glm::vec2(-10.0f, 10.0f);

	Polygon p_right;
	p_right += glm::vec2(5.0f, 5.0f);
	p_right += glm::vec2(15.0f, 5.0f);
	p_right += glm::vec2(15.0f, 15.0f);

	EXPECT_TRUE(p_left.Intersects(p_right));
	EXPECT_TRUE(p_right.Intersects(p_left));
}

TEST(Polygon, IntersectsNoHit)
{
	Polygon p_left;
	p_left += glm::vec2(-10.0f, -10.0f);
	p_left += glm::vec2(10.0f, -10.0f);
	p_left += glm::vec2(10.0f, 10.0f);
	p_left += glm::vec2(-10.0f, 10.0f);

	Polygon p_right;
	p_right += glm::vec2(0.0f, 25.0f);
	p_right += glm::vec2(-15.0f, 50.0f);
	p_right += glm::vec2(15.0f, 50.0f);

	EXPECT_FALSE(p_left.Intersects(p_right));
	EXPECT_FALSE(p_right.Intersects(p_left));
}

TEST(Polygon, BoundingBoxRectangle)
{
	Polygon p;
	p.positions.push_back(glm::vec2(50.0f, 20.0f));
	p.positions.push_back(glm::vec2(50.0f, 80.0f));
	p.positions.push_back(glm::vec2(20.0f, 80.0f));
	p.positions.push_back(glm::vec2(20.0f, 20.0f));

	BoundingBox bounds = p.GetBoundingBox();

	EXPECT_VEC2_EQ(20.0f, 20.0f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(50.0f, 80.0f, bounds.GetMaximum());
}

TEST(Polygon, BoundingBoxTriangle)
{
	Polygon p;
	p.positions.push_back(glm::vec2(-33.3f, -33.3f));
	p.positions.push_back(glm::vec2(-66.6f, 0.0f));
	p.positions.push_back(glm::vec2(66.6f, 0.0f));

	BoundingBox bounds = p.GetBoundingBox();

	EXPECT_VEC2_EQ(-66.6f, -33.3f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(66.6f, -0.0f, bounds.GetMaximum());
}

TEST(Polygon, BoundingBoxLine)
{
	Polygon p;
	p.positions.push_back(glm::vec2(-54.22f, 44.3f));
	p.positions.push_back(glm::vec2(156.9f, -76.87f));

	BoundingBox bounds = p.GetBoundingBox();

	EXPECT_VEC2_EQ(-54.22f, -76.87f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(156.9f, 44.3f, bounds.GetMaximum());
}

TEST(Polygon, BoundingBoxPoint)
{
	Polygon p;
	p.positions.push_back(glm::vec2(20.0f, 20.0f));

	BoundingBox bounds = p.GetBoundingBox();

	EXPECT_VEC2_EQ(20.0f, 20.0f, bounds.GetMinimum());
	EXPECT_VEC2_EQ(20.0f, 20.0f, bounds.GetMaximum());
}

TEST(Polygon, BoundingBoxEmpty)
{
	Polygon p;

	BoundingBox bounds = p.GetBoundingBox();

	EXPECT_FALSE(bounds.IsValid());
}