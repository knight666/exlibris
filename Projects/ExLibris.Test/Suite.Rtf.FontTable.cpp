#include "ExLibris.Test.PCH.h"

#include <RtfFontTable.h>

using namespace ExLibris;

TEST(RtfFontTable, Construct)
{
	RtfFontTable ft;

	EXPECT_EQ(0, ft.GetFontCount());
}

TEST(RtfFontTable, GetFont)
{
	RtfFontTable ft;

	RtfFont* f = ft.GetFont(16);

	EXPECT_EQ(1, ft.GetFontCount());
}

TEST(RtfFontTable, GetThreeFonts)
{
	RtfFontTable ft;

	RtfFont* f1 = ft.GetFont(8);
	RtfFont* f2 = ft.GetFont(15);
	RtfFont* f3 = ft.GetFont(7);

	EXPECT_EQ(3, ft.GetFontCount());
}

TEST(RtfFontTable, GetFontTwice)
{
	RtfFontTable ft;

	RtfFont* f1 = ft.GetFont(5);
	RtfFont* f2 = ft.GetFont(5);

	EXPECT_EQ(1, ft.GetFontCount());
	EXPECT_EQ(f2, f1);
}

TEST(RtfFontTable, GetFontInvalid)
{
	RtfFontTable ft;

	RtfFont* f = ft.GetFont(-5);

	EXPECT_EQ(nullptr, f);
	EXPECT_EQ(0, ft.GetFontCount());
}

TEST(RtfFontTable, Default)
{
	RtfFontTable ft;
	ft.SetDefault(21);

	RtfFont* f = ft.GetFont(21);
	RtfFont* f_default = ft.GetDefault();

	EXPECT_EQ(f_default, f);
}

TEST(RtfFontTable, DefaultEmpty)
{
	RtfFontTable ft;

	RtfFont* f = ft.GetDefault();

	EXPECT_EQ(nullptr, f);
}

TEST(RtfFontTable, DefaultInvalid)
{
	RtfFontTable ft;
	ft.SetDefault(-2);

	RtfFont* f = ft.GetDefault();

	EXPECT_EQ(nullptr, f);
}

TEST(RtfFontTable, DefaultValidAndThenInvalid)
{
	RtfFontTable ft;
	ft.SetDefault(5);
	ft.SetDefault(-2);

	RtfFont* f = ft.GetFont(5);
	RtfFont* f_default = ft.GetDefault();

	EXPECT_NE(nullptr, f_default);
	EXPECT_EQ(f, f_default);
}