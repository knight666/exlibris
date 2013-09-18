#include "ExLibris.Test.PCH.h"

#include "Polygon.h"

using namespace ExLibris;

TEST(Polygon, IsPositionInside)
{
	Polygon p;

	p.positions.push_back(glm::vec2(-10.0f, -10.0f));
	p.positions.push_back(glm::vec2(10.0f, -10.0f));
	p.positions.push_back(glm::vec2(10.0f, 10.0f));
	p.positions.push_back(glm::vec2(-10.0f, 10.0f));

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

	p.positions.push_back(glm::vec2(-10.0f, -10.0f));
	p.positions.push_back(glm::vec2( -8.0f, -10.0f));
	p.positions.push_back(glm::vec2(  0.0f,  -5.0f));
	p.positions.push_back(glm::vec2(  8.0f, -10.0f));
	p.positions.push_back(glm::vec2( 10.0f, -10.0f));
	p.positions.push_back(glm::vec2( 10.0f,  10.0f));
	p.positions.push_back(glm::vec2(-10.0f,  10.0f));

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

	p.positions.push_back(glm::vec2(15.0f, 99.8f));
	p.positions.push_back(glm::vec2(33.5f, 91.7f));

	EXPECT_FALSE(p.IsPositionInside(glm::vec2(14.0f, 12.0f)));
}