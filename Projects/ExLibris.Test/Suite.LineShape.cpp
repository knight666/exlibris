#include "ExLibris.Test.PCH.h"

#include "LineShape.h"

using namespace ExLibris;

TEST(LineShape, Construct)
{
	LineShape shape;

	EXPECT_EQ(0, shape.GetPolygonCount());
}

TEST(LineShape, AddPolygon)
{
	LineShape shape;

	Polygon p;
	shape.AddPolygon(p);

	EXPECT_EQ(1, shape.GetPolygonCount());
}

TEST(LineShape, ClearPolygons)
{
	LineShape shape;

	Polygon p;
	shape.AddPolygon(p);

	shape.Clear();

	EXPECT_EQ(0, shape.GetPolygonCount());
}