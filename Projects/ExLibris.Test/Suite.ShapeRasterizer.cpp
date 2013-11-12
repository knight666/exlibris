#include "ExLibris.Test.PCH.h"

#include <ShapeRasterizer.h>

using namespace ExLibris;

TEST(ShapeRasterizer, AddPolygon)
{
	ShapeRasterizer r;

	Polygon p;
	r.AddPolygon(p);

	EXPECT_EQ(1, r.GetPolygonCount());
}

TEST(ShapeRasterizer, Rasterize)
{
	ShapeRasterizer r;

	Polygon p;
	p += glm::vec2(25.0f, 25.0f);
	p += glm::vec2(50.0f, 50.0f);

	r.AddPolygon(p);

	GlyphBitmap* bm = r.Rasterize();
	EXPECT_EQ(25, bm->width);
	EXPECT_EQ(25, bm->height);
}