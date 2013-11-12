#include "ExLibris.Test.PCH.h"

#include <ShapeRasterizer.h>

using namespace ExLibris;

TEST(ShapeRasterizer, AddPolygon)
{
	Polygon p;
	p += glm::vec2(25.0f, 25.0f);
	p += glm::vec2(50.0f, 50.0f);

	ShapeRasterizer r;
	r.AddPolygon(p);

	EXPECT_EQ(1, r.GetPolygonCount());
}