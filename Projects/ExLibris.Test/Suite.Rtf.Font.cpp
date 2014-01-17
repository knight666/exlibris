#include "ExLibris.Test.PCH.h"

#include <RtfStyle.h>

#include <RtfDomDocument.h>
#include <RtfFontTable.h>
#include <RtfStyleSheet.h>

using namespace ExLibris;

TEST(RtfFont, ParseFamilyType)
{
	RtfFont f;

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "fnil";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(RtfFont::eFamilyType_Nil, f.GetFamilyType());

	t.value = "froman";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(RtfFont::eFamilyType_Roman, f.GetFamilyType());

	t.value = "fswiss";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(RtfFont::eFamilyType_Swiss, f.GetFamilyType());

	t.value = "fmodern";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(RtfFont::eFamilyType_Modern, f.GetFamilyType());

	t.value = "fscript";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(RtfFont::eFamilyType_Script, f.GetFamilyType());

	t.value = "fdecor";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(RtfFont::eFamilyType_Decor, f.GetFamilyType());

	t.value = "ftech";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(RtfFont::eFamilyType_Tech, f.GetFamilyType());

	t.value = "fbidi";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(RtfFont::eFamilyType_Bidi, f.GetFamilyType());
}

TEST(RtfFont, ParseFamilyTypeInvalid)
{
	RtfFont f;

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fblah";

	EXPECT_EQ(IRtfParseable::eResult_Propagate, f.Parse(s, t));
}

TEST(RtfFont, ParsePitch)
{
	RtfFont f;

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fprq";

	t.parameter = 0;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(RtfFont::ePitch_Default, f.GetPitch());

	t.parameter = 1;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(RtfFont::ePitch_Fixed, f.GetPitch());

	t.parameter = 2;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(RtfFont::ePitch_Variable, f.GetPitch());
}

TEST(RtfFont, ParsePitchInvalid)
{
	RtfFont f;

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fprq";

	t.parameter = -3;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, f.Parse(s, t));
	EXPECT_EQ(RtfFont::ePitch_Default, f.GetPitch());

	t.parameter = 12;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, f.Parse(s, t));
	EXPECT_EQ(RtfFont::ePitch_Default, f.GetPitch());
}

TEST(RtfFont, ParseCharacterSet)
{
	RtfFont f;

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fcharset";

	t.parameter = 0;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(eRtfCharacterSet_Ansi, f.GetCharacterSet());

	t.parameter = 82;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(eRtfCharacterSet_AppleMacintoshJohabOld, f.GetCharacterSet());

	t.parameter = 222;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(eRtfCharacterSet_Thai, f.GetCharacterSet());
}

TEST(RtfFont, ParseCharacterSetUnknown)
{
	RtfFont f;

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fcharset";

	t.parameter = 120;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, f.Parse(s, t));
	EXPECT_EQ(eRtfCharacterSet_Default, f.GetCharacterSet());
}

TEST(RtfFont, ParseCharacterSetInvalid)
{
	RtfFont f;

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fcharset";

	t.parameter =-12;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, f.Parse(s, t));
	EXPECT_EQ(eRtfCharacterSet_Default, f.GetCharacterSet());

	t.parameter = 312;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, f.Parse(s, t));
	EXPECT_EQ(eRtfCharacterSet_Default, f.GetCharacterSet());
}

TEST(RtfFont, ParseValue)
{
	RtfFont f;

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Value;
	t.value = "Arial MS";

	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));

	EXPECT_STREQ("Arial MS", f.GetName().c_str());
}