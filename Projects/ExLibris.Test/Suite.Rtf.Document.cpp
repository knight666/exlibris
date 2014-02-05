#include "ExLibris.Test.PCH.h"

#include <RtfDomDocument.h>

#include <RtfColorTable.h>
#include <RtfFontTable.h>

#include "Tools.Color.h"
#include "Tools.Parseable.h"

using namespace ExLibris;

TEST(RtfDomDocument, Construct)
{
	RtfDomDocument doc(nullptr);

	EXPECT_EQ(Rtf::eCharacterSet_Invalid, doc.GetTextFormat().GetCharacterSet());
}

TEST(RtfDomDocument, CharacterSet)
{
	RtfDomDocument doc(nullptr);

	doc.GetTextFormat().SetCharacterSet(Rtf::eCharacterSet_AppleMacintoshRoman);

	EXPECT_EQ(Rtf::eCharacterSet_AppleMacintoshRoman, doc.GetTextFormat().GetCharacterSet());
}

TEST(RtfDomDocument, ParseDefaultFont)
{
	RtfDomDocument doc(nullptr);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "deff";
	t.parameter = 2;

	EXPECT_PARSE_HANDLED(doc.Parse(s, t));

	EXPECT_EQ(doc.GetFontTable()->GetFont(2), doc.GetFontTable()->GetDefault());
}

TEST(RtfDomDocument, ParseFontTable)
{
	RtfDomDocument doc(nullptr);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "fonttbl";
	EXPECT_PARSE_HANDLED(doc.Parse(s, t));

	EXPECT_EQ(doc.GetFontTable(), s.GetTarget());

	t.value = "f";
	t.parameter = 5;
	EXPECT_PARSE_HANDLED(s.GetTarget()->Parse(s, t));

	t.value = "froman";
	EXPECT_PARSE_HANDLED(s.GetTarget()->Parse(s, t));

	t.type = RtfToken::eParseType_Value;
	t.value = "Comical";
	EXPECT_PARSE_HANDLED(s.GetTarget()->Parse(s, t));

	RtfFont* font = doc.GetFontTable()->GetFont(5);
	EXPECT_EQ(Rtf::eFamilyType_Roman, font->GetFamilyType());
	EXPECT_STREQ("Comical", font->GetName().c_str());
}

TEST(RtfDomDocument, ParseColorTable)
{
	RtfDomDocument doc(nullptr);

	RtfParserState s;

	RtfToken t;

	t.type = RtfToken::eParseType_GroupOpen;
	EXPECT_PARSE_HANDLED(doc.Parse(s, t));

	t.type = RtfToken::eParseType_Command;
	t.value = "colortbl";
	EXPECT_PARSE_HANDLED(doc.Parse(s, t));

	EXPECT_EQ(doc.GetColorTable(), s.GetTarget());

	t.value = "red";
	t.parameter = 128;
	EXPECT_PARSE_HANDLED(s.GetTarget()->Parse(s, t));

	t.value = "green";
	t.parameter = 128;
	EXPECT_PARSE_HANDLED(s.GetTarget()->Parse(s, t));

	t.value = "blue";
	t.parameter = 128;
	EXPECT_PARSE_HANDLED(s.GetTarget()->Parse(s, t));

	t.type = RtfToken::eParseType_GroupClose;
	EXPECT_PARSE_HANDLED(doc.Parse(s, t));

	RtfColor* color = doc.GetColorTable()->GetColor(0);
	EXPECT_COLOR_RGB(128, 128, 128, *color);
}

TEST(RtfDomDocument, ParseStyleSheet)
{
	RtfDomDocument doc(nullptr);

	RtfParserState s;

	RtfToken t;

	t.type = RtfToken::eParseType_GroupOpen;
	EXPECT_PARSE_HANDLED(doc.Parse(s, t));

	t.type = RtfToken::eParseType_Command;
	t.value = "stylesheet";
	EXPECT_PARSE_HANDLED(doc.Parse(s, t));

	EXPECT_EQ(doc.GetStyleSheet(), s.GetTarget());

	t.value = "s";
	t.parameter = 0;
	EXPECT_PARSE_HANDLED(s.GetTarget()->Parse(s, t));

	t.value = "kerning";
	t.parameter = 1;
	EXPECT_PARSE_HANDLED(s.GetTarget()->Parse(s, t));

	t.type = RtfToken::eParseType_Value;
	t.value = "Awesome";
	EXPECT_PARSE_HANDLED(s.GetTarget()->Parse(s, t));

	RtfStyle* st = doc.GetStyleSheet()->GetStyle(0);
	EXPECT_STREQ("Awesome", st->GetName().c_str());
	EXPECT_TRUE(st->GetTextFormat().IsKerningEnabled());
}