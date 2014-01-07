#include "ExLibris.Test.PCH.h"

#include <RtfColorTable.h>

using namespace ExLibris;

TEST(RtfColorTable, Construct)
{
	RtfColorTable ct;

	EXPECT_EQ(0, ct.GetColorCount());
}

TEST(RtfColorTable, AddColor)
{
	RtfColorTable ct;
	ct.AddColor(RtfColor(126, 12, 88));

	EXPECT_EQ(1, ct.GetColorCount());
}

TEST(RtfColorTable, AddThreeColors)
{
	RtfColorTable ct;
	ct.AddColor(RtfColor(51, 18, 99));
	ct.AddColor(RtfColor(36, 77, 17));
	ct.AddColor(RtfColor(5, 199, 204));

	EXPECT_EQ(3, ct.GetColorCount());
}

TEST(RtfColorTable, GetColor)
{
	RtfColorTable ct;
	ct.AddColor(RtfColor(34, 12, 99));

	RtfColor c = ct.GetColor(0);

	EXPECT_EQ(34, c.r);
	EXPECT_EQ(12, c.g);
	EXPECT_EQ(99, c.b);
}

TEST(RtfColorTable, GetColorInvalidIndex)
{
	RtfColorTable ct;
	ct.AddColor(RtfColor(138, 98, 115));
	ct.AddColor(RtfColor(12, 12, 22));

	RtfColor c = ct.GetColor(2);

	EXPECT_EQ(0, c.r);
	EXPECT_EQ(0, c.g);
	EXPECT_EQ(0, c.b);
}

TEST(RtfColorTable, GetColorEmpty)
{
	RtfColorTable ct;

	RtfColor c = ct.GetColor(11);

	EXPECT_EQ(0, c.r);
	EXPECT_EQ(0, c.g);
	EXPECT_EQ(0, c.b);
}

TEST(RtfColorTable, GetColorDefault)
{
	RtfColorTable ct;

	RtfColor default(117, 8, 88);
	RtfColor c = ct.GetColor(11, default);

	EXPECT_EQ(117, c.r);
	EXPECT_EQ(8, c.g);
	EXPECT_EQ(88, c.b);
}

TEST(RtfColorTable, TryGetColor)
{
	RtfColorTable ct;
	ct.AddColor(RtfColor(17, 88, 12));

	RtfColor c;
	EXPECT_TRUE(ct.TryGetColor(c, 0));

	EXPECT_EQ(17, c.r);
	EXPECT_EQ(88, c.g);
	EXPECT_EQ(12, c.b);
}

TEST(RtfColorTable, TryGetColorInvalidIndex)
{
	RtfColorTable ct;
	ct.AddColor(RtfColor(44, 66, 44));
	ct.AddColor(RtfColor(1, 3, 4));

	RtfColor c;
	EXPECT_FALSE(ct.TryGetColor(c, 15));

	EXPECT_EQ(0, c.r);
	EXPECT_EQ(0, c.g);
	EXPECT_EQ(0, c.b);
}

TEST(RtfColorTable, TryGetColorEmpty)
{
	RtfColorTable ct;

	RtfColor c;
	EXPECT_FALSE(ct.TryGetColor(c, 1));

	EXPECT_EQ(0, c.r);
	EXPECT_EQ(0, c.g);
	EXPECT_EQ(0, c.b);
}