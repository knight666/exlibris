#include "ExLibris.Test.PCH.h"

#include <RtfDomDocument.h>

#include <RtfColorTable.h>
#include <RtfFontTable.h>

#include "Tools.Color.h"

using namespace ExLibris;

TEST(RtfDomDocument, Construct)
{
	RtfDomDocument doc(nullptr);

	EXPECT_EQ(eRtfCharacterSet_Invalid, doc.GetTextFormat().GetCharacterSet());
}

TEST(RtfDomDocument, CharacterSet)
{
	RtfDomDocument doc(nullptr);

	doc.GetTextFormat().SetCharacterSet(eRtfCharacterSet_AppleMacintoshRoman);

	EXPECT_EQ(eRtfCharacterSet_AppleMacintoshRoman, doc.GetTextFormat().GetCharacterSet());
}

TEST(RtfDomDocument, ParseDefaultFont)
{
	RtfDomDocument doc(nullptr);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "deff";
	t.parameter = 2;

	EXPECT_EQ(IRtfParseable::eResult_Handled, doc.Parse(s, t));

	EXPECT_EQ(doc.GetFontTable()->GetFont(2), doc.GetFontTable()->GetDefault());
}

TEST(RtfDomDocument, ParseFontTable)
{
	RtfDomDocument doc(nullptr);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "fonttbl";
	EXPECT_EQ(IRtfParseable::eResult_Handled, doc.Parse(s, t));

	EXPECT_EQ(doc.GetFontTable(), s.target_current);

	t.value = "f";
	t.parameter = 5;
	EXPECT_EQ(IRtfParseable::eResult_Handled, s.target_current->Parse(s, t));

	t.value = "froman";
	EXPECT_EQ(IRtfParseable::eResult_Handled, s.target_current->Parse(s, t));

	t.type = RtfToken::eParseType_Value;
	t.value = "Comical";
	EXPECT_EQ(IRtfParseable::eResult_Handled, s.target_current->Parse(s, t));

	RtfFont* font = doc.GetFontTable()->GetFont(5);
	EXPECT_EQ(RtfFont::eFamilyType_Roman, font->GetFamilyType());
	EXPECT_STREQ("Comical", font->GetName().c_str());
}

TEST(RtfDomDocument, ParseColorTable)
{
	RtfDomDocument doc(nullptr);

	RtfParserState s;

	RtfToken t;

	t.type = RtfToken::eParseType_GroupOpen;
	EXPECT_EQ(IRtfParseable::eResult_Handled, doc.Parse(s, t));

	t.type = RtfToken::eParseType_Command;
	t.value = "colortbl";
	EXPECT_EQ(IRtfParseable::eResult_Handled, doc.Parse(s, t));

	EXPECT_EQ(doc.GetColorTable(), s.target_current);

	t.value = "red";
	t.parameter = 128;
	EXPECT_EQ(IRtfParseable::eResult_Handled, s.target_current->Parse(s, t));

	t.value = "green";
	t.parameter = 128;
	EXPECT_EQ(IRtfParseable::eResult_Handled, s.target_current->Parse(s, t));

	t.value = "blue";
	t.parameter = 128;
	EXPECT_EQ(IRtfParseable::eResult_Handled, s.target_current->Parse(s, t));

	t.type = RtfToken::eParseType_GroupClose;
	EXPECT_EQ(IRtfParseable::eResult_Handled, doc.Parse(s, t));

	RtfColor* color = doc.GetColorTable()->GetColor(0);
	EXPECT_COLOR_RGB(128, 128, 128, *color);
}