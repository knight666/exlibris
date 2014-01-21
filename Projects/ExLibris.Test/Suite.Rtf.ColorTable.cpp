#include "ExLibris.Test.PCH.h"

#include <RtfColorTable.h>

#include <RtfDomDocument.h>

#include "Tools.Color.h"

using namespace ExLibris;

TEST(RtfColorTable, Construct)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	EXPECT_EQ(1, ct.GetColorCount());
}

TEST(RtfColorTable, GetDefaultColor)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	ASSERT_NE(nullptr, ct.GetDefaultColor());
	EXPECT_COLOR_RGB(0, 0, 0, *ct.GetDefaultColor());
}

TEST(RtfColorTable, SetDefaultColor)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);
	RtfColor* c = ct.GetColor(43);

	ct.SetDefaultColor(43);

	ASSERT_NE(nullptr, ct.GetDefaultColor());
	EXPECT_EQ(c, ct.GetDefaultColor());
}

TEST(RtfColorTable, SetDefaultColorNew)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);
	*ct.GetDefaultColor() = RtfColor(54, 12, 99);

	ct.SetDefaultColor(11);

	ASSERT_NE(nullptr, ct.GetDefaultColor());
	EXPECT_COLOR_RGB(54, 12, 99, *ct.GetDefaultColor());
}

TEST(RtfColorTable, SetDefaultColorInvalidIndex)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);
	
	ct.SetDefaultColor(-12);

	ASSERT_NE(nullptr, ct.GetDefaultColor());
	EXPECT_EQ(ct.GetDefaultColor(), ct.GetColor(0));
}

TEST(RtfColorTable, GetColor)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	RtfColor* color = ct.GetColor(81);

	ASSERT_NE(nullptr, color);
	EXPECT_COLOR_RGB(0, 0, 0, *color);

	EXPECT_EQ(2, ct.GetColorCount());
}

TEST(RtfColorTable, GetColorTwice)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	RtfColor* c1 = ct.GetColor(81);
	RtfColor* c2 = ct.GetColor(81);

	EXPECT_EQ(c1, c2);

	EXPECT_EQ(2, ct.GetColorCount());
}

TEST(RtfColorTable, GetColorSetToDefault)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	*ct.GetDefaultColor() = RtfColor(16, 99, 12);

	RtfColor* color = ct.GetColor(77);

	ASSERT_NE(nullptr, color);
	EXPECT_COLOR_RGB(16, 99, 12, *color);

	EXPECT_EQ(2, ct.GetColorCount());
}

TEST(RtfColorTable, GetColorInvalidIndex)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	EXPECT_EQ(nullptr, ct.GetColor(-112));

	EXPECT_EQ(1, ct.GetColorCount());
}

TEST(RtfColorTable, AddColor)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);
	RtfColor* c = ct.AddColor(RtfColor(126, 12, 88));

	ASSERT_NE(nullptr, c);
	EXPECT_COLOR_RGB(126, 12, 88, *c);
	EXPECT_EQ(c, ct.GetColor(1));
	EXPECT_EQ(2, ct.GetColorCount());
}

TEST(RtfColorTable, AddThreeColors)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);
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
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);
	ct.GetColor(117);

	ct.AddColor(RtfColor(126, 12, 88));

	EXPECT_COLOR_RGB(126, 12, 88, *ct.GetColor(118));
}

TEST(RtfColorTable, ParseColorTable)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "colortbl";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ct.Parse(s, t));
}

TEST(RtfColorTable, ParseColorTableAndClose)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	RtfParserState s;

	RtfToken t;

	t.type = RtfToken::eParseType_GroupOpen;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ct.Parse(s, t));

	t.type = RtfToken::eParseType_Command;
	t.value = "colortbl";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ct.Parse(s, t));

	t.type = RtfToken::eParseType_GroupClose;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ct.Parse(s, t));

	EXPECT_EQ(nullptr, s.GetTarget());
	EXPECT_EQ(0, s.GetGroupIndex());
}

TEST(RtfColorTable, ParseColorTableUnhandled)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "colortbl";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ct.Parse(s, t));

	t.value = "fblah";
	EXPECT_EQ(IRtfParseable::eResult_Invalid, ct.Parse(s, t));
}

TEST(RtfColorTable, ParseColor)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "colortbl";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ct.Parse(s, t));

	t.value = "red";
	t.parameter = 12;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ct.Parse(s, t));

	t.value = "green";
	t.parameter = 133;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ct.Parse(s, t));

	t.value = "blue";
	t.parameter = 15;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ct.Parse(s, t));

	EXPECT_COLOR_RGB(12, 133, 15, *ct.GetColor(0));
}

TEST(RtfColorTable, ParseColorInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "colortbl";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ct.Parse(s, t));

	t.value = "red";
	t.parameter = -15;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, ct.Parse(s, t));

	t.value = "red";
	t.parameter = 1239;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, ct.Parse(s, t));

	t.value = "green";
	t.parameter = -98;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, ct.Parse(s, t));

	t.value = "green";
	t.parameter = 977;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, ct.Parse(s, t));

	t.value = "blue";
	t.parameter = -6;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, ct.Parse(s, t));

	t.value = "blue";
	t.parameter = 531;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, ct.Parse(s, t));
}

TEST(RtfColorTable, ParseColorWithoutColorTable)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "red";
	t.parameter = 125;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, ct.Parse(s, t));
}

TEST(RtfColorTable, ParseUnhandled)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "colortbl";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ct.Parse(s, t));

	t.value = "awesomium";
	EXPECT_EQ(IRtfParseable::eResult_Invalid, ct.Parse(s, t));
}

TEST(RtfColorTable, ParseUnhandledWithoutColorTable)
{
	RtfDomDocument doc(nullptr);
	RtfColorTable ct(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "magnificent";
	EXPECT_EQ(IRtfParseable::eResult_Propagate, ct.Parse(s, t));
}