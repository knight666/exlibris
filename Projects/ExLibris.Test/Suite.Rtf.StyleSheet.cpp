#include "ExLibris.Test.PCH.h"

#include <RtfStyleSheet.h>

#include <RtfDomDocument.h>

#include "Tools.Parseable.h"

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

	EXPECT_PARSE_HANDLED(ss.Parse(s, t));
}

TEST(RtfStyleSheet, ParseStylesheetAndClose)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;

	RtfToken t;

	t.type = RtfToken::eParseType_GroupOpen;
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

	t.type = RtfToken::eParseType_Command;
	t.value = "stylesheet";
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

	t.type = RtfToken::eParseType_GroupClose;
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

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
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

	t.value = "s";
	t.parameter = 0;
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

	EXPECT_EQ(&ss, s.GetTarget());
}

TEST(RtfStyleSheet, ParseTwoStyles)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;

	RtfToken t;

	t.type = RtfToken::eParseType_Command;
	t.value = "stylesheet";
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

	t.type = RtfToken::eParseType_Command;
	t.value = "s";
	t.parameter = 0;
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

	t.type = RtfToken::eParseType_Value;
	t.value = "Ganymede";
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

	t.type = RtfToken::eParseType_Command;
	t.value = "s";
	t.parameter = 1;
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

	t.type = RtfToken::eParseType_Value;
	t.value = "Europa";
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

	EXPECT_STREQ("Ganymede", ss.GetStyle(0)->GetName().c_str());
	EXPECT_STREQ("Europa", ss.GetStyle(1)->GetName().c_str());
}

TEST(RtfStyleSheet, ParseStyleInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "stylesheet";
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

	t.value = "s";
	t.parameter = -97;
	EXPECT_PARSE_INVALID(ss.Parse(s, t));
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

	EXPECT_PARSE_INVALID(ss.Parse(s, t));
}

TEST(RtfStyleSheet, ParseUnhandled)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "stylesheet";
	EXPECT_PARSE_HANDLED(ss.Parse(s, t));

	t.value = "pard";
	EXPECT_PARSE_PROPAGATE(ss.Parse(s, t));
}