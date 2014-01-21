#include "ExLibris.Test.PCH.h"

#include <RtfStyle.h>

#include <RtfDomDocument.h>
#include <RtfFontTable.h>
#include <RtfStyleSheet.h>

using namespace ExLibris;

TEST(RtfStyle, Construct)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	EXPECT_STREQ("", st.GetName().c_str());
	EXPECT_EQ(&st, st.GetNextParagraphStyle());
}

TEST(RtfStyle, ParseStyle)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "s";
	t.parameter = 1;

	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));
}

TEST(RtfStyleSheet, ParseStyleAndClose)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	RtfParserState s;
	s.PushGroup();

	RtfToken t;

	t.type = RtfToken::eParseType_GroupOpen;
	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));

	t.type = RtfToken::eParseType_Command;
	t.value = "s";
	t.parameter = 1;
	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));

	t.type = RtfToken::eParseType_GroupClose;
	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));

	EXPECT_EQ(nullptr, s.GetTarget());
	EXPECT_EQ(0, s.GetGroupIndex());
}

TEST(RtfStyle, ParseNextParagraphStyle)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "snext";
	t.parameter = 1;

	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));

	EXPECT_EQ(ss.GetStyle(1), st.GetNextParagraphStyle());
}

TEST(RtfStyle, ParseCharacterEncoding)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.parameter = -1;

	t.value = "loch";
	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));

	t.value = "hich";
	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));

	t.value = "dbch";
	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));
}

TEST(RtfStyle, ParseAssociatedFontProperty)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.parameter = -1;

	t.value = "loch";
	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));

	t.value = "af";
	t.parameter = 12;
	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));

	RtfAssociatedProperties* props = st.GetPropertiesForCharacterEncoding(eRtfCharacterEncoding_SingleByteLowAnsi);
	ASSERT_NE(nullptr, props);
	
	EXPECT_EQ(doc.GetFontTable()->GetFont(12), props->GetFont());
}

TEST(RtfStyle, ParseAssociatedFontPropertyNotSet)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "af";
	t.parameter = 12;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, st.Parse(s, t));
}

TEST(RtfStyle, ParseAssociatedFontPropertyInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "afs";
	t.parameter = -128;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, st.Parse(s, t));
}

TEST(RtfStyle, ParseTextFormatProperty)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fs";
	t.parameter = 180;

	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));

	EXPECT_FLOAT_EQ(90.0f, st.GetTextFormat().GetFontSize());
}

TEST(RtfStyle, ParseTextFormatPropertyInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fs";
	t.parameter = -129;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, st.Parse(s, t));

	EXPECT_FLOAT_EQ(12.0f, st.GetTextFormat().GetFontSize());
}

TEST(RtfStyle, ParseName)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Value;
	t.value = "Ring-a-ding";

	EXPECT_EQ(IRtfParseable::eResult_Handled, st.Parse(s, t));

	EXPECT_STREQ("Ring-a-ding", st.GetName().c_str());
}

TEST(RtfStyle, ParseUnhandled)
{
	RtfDomDocument doc(nullptr);
	RtfStyleSheet ss(doc);
	RtfStyle st(ss, doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "brillo";
	t.parameter = -1;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, st.Parse(s, t));
}