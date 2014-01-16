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

	RtfParserGroup root;
	root.index = 0;

	RtfParserState s;
	s.group_current = &root;

	RtfToken t;

	t.type = RtfToken::eParseType_GroupOpen;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ss.Parse(s, t));

	t.type = RtfToken::eParseType_Command;
	t.value = "stylesheet";
	EXPECT_EQ(IRtfParseable::eResult_Handled, ss.Parse(s, t));

	t.type = RtfToken::eParseType_GroupClose;
	EXPECT_EQ(IRtfParseable::eResult_Handled, ss.Parse(s, t));

	EXPECT_EQ(0, s.targets.size());
	EXPECT_EQ(nullptr, s.target_current);
	EXPECT_EQ(0, s.group_index);
	EXPECT_EQ(&root, s.group_current);
}

TEST(RtfStyleSheet, ParseStyle)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "s";
	t.parameter = 0;

	EXPECT_EQ(IRtfParseable::eResult_Handled, ss.Parse(s, t));

	EXPECT_EQ(1, s.targets.size());
	EXPECT_EQ(ss.GetStyle(0), s.target_current);
}

TEST(RtfStyleSheet, ParseStyleInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "s";
	t.parameter = -97;

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