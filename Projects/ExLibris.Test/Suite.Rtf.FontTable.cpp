#include "ExLibris.Test.PCH.h"

#include <RtfFontTable.h>

#include <RtfDomDocument.h>

using namespace ExLibris;

TEST(RtfFontTable, Construct)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	EXPECT_EQ(0, ft.GetFontCount());
}

TEST(RtfFontTable, GetFont)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfFont* f = ft.GetFont(16);

	EXPECT_EQ(1, ft.GetFontCount());
}

TEST(RtfFontTable, GetThreeFonts)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfFont* f1 = ft.GetFont(8);
	RtfFont* f2 = ft.GetFont(15);
	RtfFont* f3 = ft.GetFont(7);

	EXPECT_EQ(3, ft.GetFontCount());
}

TEST(RtfFontTable, GetFontTwice)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfFont* f1 = ft.GetFont(5);
	RtfFont* f2 = ft.GetFont(5);

	EXPECT_EQ(1, ft.GetFontCount());
	EXPECT_EQ(f2, f1);
}

TEST(RtfFontTable, GetFontInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfFont* f = ft.GetFont(-5);

	EXPECT_EQ(nullptr, f);
	EXPECT_EQ(0, ft.GetFontCount());
}

TEST(RtfFontTable, Default)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);
	ft.SetDefault(21);

	RtfFont* f = ft.GetFont(21);
	RtfFont* f_default = ft.GetDefault();

	EXPECT_EQ(f_default, f);
}

TEST(RtfFontTable, DefaultEmpty)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfFont* f = ft.GetDefault();

	EXPECT_EQ(nullptr, f);
}

TEST(RtfFontTable, DefaultInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);
	ft.SetDefault(-2);

	RtfFont* f = ft.GetDefault();

	EXPECT_EQ(nullptr, f);
}

TEST(RtfFontTable, DefaultValidAndThenInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);
	ft.SetDefault(5);
	ft.SetDefault(-2);

	RtfFont* f = ft.GetFont(5);
	RtfFont* f_default = ft.GetDefault();

	EXPECT_NE(nullptr, f_default);
	EXPECT_EQ(f, f_default);
}

TEST(RtfFontTable, ParseFontTable)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fonttbl";

	EXPECT_EQ(IRtfParseable::eResult_Handled, ft.Parse(s, t));
}

TEST(RtfFontTable, ParseFontTableAndClose)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfParserState s;

	RtfToken t;

	t.type = RtfToken::eParseType_GroupOpen;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ft.Parse(s, t));

	t.type = RtfToken::eParseType_Command;
	t.value = "fonttbl";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ft.Parse(s, t));

	t.type = RtfToken::eParseType_GroupClose;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ft.Parse(s, t));

	EXPECT_EQ(nullptr, s.GetTarget());
	EXPECT_EQ(0, s.GetGroupIndex());
}

TEST(RtfFontTable, ParseFont)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "fonttbl";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ft.Parse(s, t));

	t.value = "f";
	t.parameter = 5;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ft.Parse(s, t));

	EXPECT_EQ(ft.GetFont(5), s.GetTarget());
}

TEST(RtfFontTable, ParseFontInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	
	t.value = "fonttbl";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ft.Parse(s, t));

	t.value = "f";
	t.parameter = -12;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, ft.Parse(s, t));
}

TEST(RtfFontTable, ParseFontWithoutTable)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "f";
	t.parameter = 12;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, ft.Parse(s, t));
}

TEST(RtfFontTable, ParseDefaultFont)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "deff";
	t.parameter = 12;

	EXPECT_EQ(IRtfParseable::eResult_Handled, ft.Parse(s, t));

	EXPECT_EQ(ft.GetFont(12), ft.GetDefault());
}

TEST(RtfFontTable, ParseDefaultFontInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "deff";
	t.parameter = -18;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, ft.Parse(s, t));
}

TEST(RtfFontTable, ParseUnhandled)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "b";
	EXPECT_EQ(IRtfParseable::eResult_Propagate, ft.Parse(s, t));
}