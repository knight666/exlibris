#include "ExLibris.Test.PCH.h"

#include <RtfStyle.h>

#include <RtfDomDocument.h>
#include <RtfFontTable.h>
#include <RtfStyleSheet.h>

using namespace ExLibris;

TEST(RtfFont, ParseFamilyType)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);
	RtfFont f(ft);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "fnil";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::eFamilyType_Nil, f.GetFamilyType());

	t.value = "froman";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::eFamilyType_Roman, f.GetFamilyType());

	t.value = "fswiss";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::eFamilyType_Swiss, f.GetFamilyType());

	t.value = "fmodern";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::eFamilyType_Modern, f.GetFamilyType());

	t.value = "fscript";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::eFamilyType_Script, f.GetFamilyType());

	t.value = "fdecor";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::eFamilyType_Decor, f.GetFamilyType());

	t.value = "ftech";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::eFamilyType_Tech, f.GetFamilyType());

	t.value = "fbidi";
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::eFamilyType_Bidi, f.GetFamilyType());
}

TEST(RtfFont, ParseFamilyTypeInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);
	RtfFont f(ft);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fblah";

	EXPECT_EQ(IRtfParseable::eResult_Propagate, f.Parse(s, t));
}

TEST(RtfFont, ParsePitch)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);
	RtfFont f(ft);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fprq";

	t.parameter = 0;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::ePitch_Default, f.GetPitch());

	t.parameter = 1;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::ePitch_Fixed, f.GetPitch());

	t.parameter = 2;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::ePitch_Variable, f.GetPitch());
}

TEST(RtfFont, ParsePitchInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);
	RtfFont f(ft);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fprq";

	t.parameter = -3;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, f.Parse(s, t));
	EXPECT_EQ(Rtf::ePitch_Default, f.GetPitch());

	t.parameter = 12;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, f.Parse(s, t));
	EXPECT_EQ(Rtf::ePitch_Default, f.GetPitch());
}

TEST(RtfFont, ParseCharacterSet)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);
	RtfFont f(ft);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fcharset";

	t.parameter = 0;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::eCharacterSet_Ansi, f.GetCharacterSet());

	t.parameter = 82;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::eCharacterSet_AppleMacintoshJohabOld, f.GetCharacterSet());

	t.parameter = 222;
	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));
	EXPECT_EQ(Rtf::eCharacterSet_Thai, f.GetCharacterSet());
}

TEST(RtfFont, ParseCharacterSetUnknown)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);
	RtfFont f(ft);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fcharset";

	t.parameter = 120;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, f.Parse(s, t));
	EXPECT_EQ(Rtf::eCharacterSet_Default, f.GetCharacterSet());
}

TEST(RtfFont, ParseCharacterSetInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);
	RtfFont f(ft);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fcharset";

	t.parameter =-12;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, f.Parse(s, t));
	EXPECT_EQ(Rtf::eCharacterSet_Default, f.GetCharacterSet());

	t.parameter = 312;
	EXPECT_EQ(IRtfParseable::eResult_Invalid, f.Parse(s, t));
	EXPECT_EQ(Rtf::eCharacterSet_Default, f.GetCharacterSet());
}

TEST(RtfFont, ParseValue)
{
	RtfDomDocument doc(nullptr);
	RtfFontTable ft(doc);
	RtfFont f(ft);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Value;
	t.value = "Arial MS";

	EXPECT_EQ(IRtfParseable::eResult_Handled, f.Parse(s, t));

	EXPECT_STREQ("Arial MS", f.GetName().c_str());
}