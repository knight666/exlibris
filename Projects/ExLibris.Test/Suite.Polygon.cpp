#include "ExLibris.Test.PCH.h"

#include "Polygon.h"

using namespace ExLibris;

TEST(Polygon, TriangulateEmpty)
{
	Polygon polygon;

	TriangleList list = polygon.Triangulate();

	ASSERT_EQ(0, list.positions.size());
}