#include "ExLibris.Test.PCH.h"

#include <RtfColorTable.h>

#include "Tools.Color.h"

using namespace ExLibris;

TEST(RtfColorTable, Construct)
{
	RtfColorTable ct;

	EXPECT_EQ(1, ct.GetColorCount());
}

TEST(RtfColorTable, GetDefaultColor)
{
	RtfColorTable ct;

	ASSERT_NE(nullptr, ct.GetDefaultColor());
	EXPECT_COLOR_RGB(0, 0, 0, *ct.GetDefaultColor());
}

TEST(RtfColorTable, SetDefaultColor)
{
	RtfColorTable ct;
	RtfColor* c = ct.GetColor(43);

	ct.SetDefaultColor(43);

	ASSERT_NE(nullptr, ct.GetDefaultColor());
	EXPECT_EQ(c, ct.GetDefaultColor());
}

TEST(RtfColorTable, SetDefaultColorNew)
{
	RtfColorTable ct;
	*ct.GetDefaultColor() = RtfColor(54, 12, 99);

	ct.SetDefaultColor(11);

	ASSERT_NE(nullptr, ct.GetDefaultColor());
	EXPECT_COLOR_RGB(54, 12, 99, *ct.GetDefaultColor());
}

TEST(RtfColorTable, SetDefaultColorInvalidIndex)
{
	RtfColorTable ct;
	
	ct.SetDefaultColor(-12);

	ASSERT_NE(nullptr, ct.GetDefaultColor());
	EXPECT_EQ(ct.GetDefaultColor(), ct.GetColor(0));
}

TEST(RtfColorTable, GetColor)
{
	RtfColorTable ct;

	RtfColor* color = ct.GetColor(81);

	ASSERT_NE(nullptr, color);
	EXPECT_COLOR_RGB(0, 0, 0, *color);

	EXPECT_EQ(2, ct.GetColorCount());
}

TEST(RtfColorTable, GetColorTwice)
{
	RtfColorTable ct;

	RtfColor* c1 = ct.GetColor(81);
	RtfColor* c2 = ct.GetColor(81);

	EXPECT_EQ(c1, c2);

	EXPECT_EQ(2, ct.GetColorCount());
}

TEST(RtfColorTable, GetColorSetToDefault)
{
	RtfColorTable ct;

	*ct.GetDefaultColor() = RtfColor(16, 99, 12);

	RtfColor* color = ct.GetColor(77);

	ASSERT_NE(nullptr, color);
	EXPECT_COLOR_RGB(16, 99, 12, *color);

	EXPECT_EQ(2, ct.GetColorCount());
}

TEST(RtfColorTable, GetColorInvalidIndex)
{
	RtfColorTable ct;

	EXPECT_EQ(nullptr, ct.GetColor(-112));

	EXPECT_EQ(1, ct.GetColorCount());
}

TEST(RtfColorTable, AddColor)
{
	RtfColorTable ct;
	RtfColor* c = ct.AddColor(RtfColor(126, 12, 88));

	ASSERT_NE(nullptr, c);
	EXPECT_COLOR_RGB(126, 12, 88, *c);
	EXPECT_EQ(c, ct.GetColor(1));
	EXPECT_EQ(2, ct.GetColorCount());
}

TEST(RtfColorTable, AddThreeColors)
{
	RtfColorTable ct;
	ct.AddColor(RtfColor(51, 18, 99));
	ct.AddColor(RtfColor(36, 77, 17));
	ct.AddColor(RtfColor(5, 199, 204));

	EXPECT_EQ(4, ct.GetColorCount());
	ASSERT_NE(nullptr, ct.GetColor(1));
	EXPECT_COLOR_RGB(51, 18, 99, *ct.GetColor(1));
	ASSERT_NE(nullptr, ct.GetColor(2));
	EXPECT_COLOR_RGB(36, 77, 17, *ct.GetColor(2));
	ASSERT_NE(nullptr, ct.GetColor(3));
	EXPECT_COLOR_RGB(5, 199, 204, *ct.GetColor(3));
}

TEST(RtfColorTable, AddColorNextIndex)
{
	RtfColorTable ct;
	ct.GetColor(117);

	ct.AddColor(RtfColor(126, 12, 88));

	EXPECT_COLOR_RGB(126, 12, 88, *ct.GetColor(118));
}