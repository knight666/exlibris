#include "ExLibris.Test.PCH.h"

#include <RtfStyleSheet.h>

#include <RtfDomDocument.h>

using namespace ExLibris;

TEST(RtfStyleSheet, ParseStylesheet)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "stylesheet";
	t.parameter = 0;

	EXPECT_EQ(IRtfParseable::eResult_Handled, ss.Parse(s, t));
}

TEST(RtfStyleSheet, ParseStylesheetAndClose)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;
	s.PushGroup();

	RtfToken t;

	t.type = RtfToken::eParseType_GroupOpen;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ss.Parse(s, t));

	t.type = RtfToken::eParseType_Command;
	t.value = "stylesheet";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ss.Parse(s, t));

	t.type = RtfToken::eParseType_GroupClose;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ss.Parse(s, t));

	EXPECT_EQ(nullptr, s.GetTarget());
	EXPECT_EQ(0, s.GetGroupIndex());
}

TEST(RtfStyleSheet, ParseStyle)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "stylesheet";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ss.Parse(s, t));

	t.value = "s";
	t.parameter = 0;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ss.Parse(s, t));

	EXPECT_EQ(ss.GetStyle(0), s.GetTarget());
}

TEST(RtfStyleSheet, ParseStyleInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "stylesheet";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ss.Parse(s, t));

	t.value = "s";
	t.parameter = -97;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, ss.Parse(s, t));
}

TEST(RtfStyleSheet, ParseStyleWithoutSheet)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "s";
	t.parameter = 5;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, ss.Parse(s, t));
}

TEST(RtfStyleSheet, ParseUnhandled)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "pard";

	EXPECT_EQ(IRtfParseable::eResult_Propagate, ss.Parse(s, t));
}