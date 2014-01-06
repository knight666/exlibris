#include "ExLibris.Test.PCH.h"

#include <TextParserRtf.h>

using namespace ExLibris;

TEST(TextParserRtf, HelloWorld)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi{\\fonttbl\\f0\\fswiss Helvetica;}\\f0\\pard\nHello World!\\par\n}";

	RtfDomDocument* doc = parser.ParseDocument(&input);

	ASSERT_NE(nullptr, doc);

	EXPECT_EQ(RtfFont::eFamilyType_Swiss, doc->GetFont(0).family);
	EXPECT_STREQ("Helvetica", doc->GetFont(0).name.c_str());

	RtfDomElement* root = doc->GetRootElement();

	EXPECT_EQ(eRtfCharacterSet_Ansi, root->TextFormat.character_set);
	EXPECT_STREQ("Hello World!", root->InnerText.c_str());

	EXPECT_EQ(1, root->GetChildrenCount());
}

TEST(TextParserRtf, UseDefaultFont)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi\\deff0{\\fonttbl\\f0Robotica;}Bleep bloop.}";

	RtfDomDocument* doc = parser.ParseDocument(&input);

	ASSERT_NE(nullptr, doc);

	RtfDomElement* root = doc->GetRootElement();

	ASSERT_NE(nullptr, root->TextFormat.font);

	EXPECT_EQ(RtfFont::eFamilyType_Nil, root->TextFormat.font->family);
	EXPECT_STREQ("Robotica", root->TextFormat.font->name.c_str());
	EXPECT_STREQ("Bleep bloop.", root->InnerText.c_str());
}

TEST(TextParserRtf, FontSetPitch)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi";
	input << "{\\fonttbl";
	input << "{\\f0\\fprq1 Magnified;}";
	input << "{\\f1\\fprq0 Simplified;}";
	input << "{\\f2\\fprq2 Testified;}";
	input << "}}";

	RtfDomDocument* doc = parser.ParseDocument(&input);

	ASSERT_NE(nullptr, doc);

	EXPECT_STREQ("Magnified", doc->GetFont(0).name.c_str());
	EXPECT_EQ(RtfFont::ePitch_Fixed, doc->GetFont(0).pitch);

	EXPECT_STREQ("Simplified", doc->GetFont(1).name.c_str());
	EXPECT_EQ(RtfFont::ePitch_Default, doc->GetFont(1).pitch);

	EXPECT_STREQ("Testified", doc->GetFont(2).name.c_str());
	EXPECT_EQ(RtfFont::ePitch_Variable, doc->GetFont(2).pitch);
}

TEST(TextParserRtf, FontSetCharacterSet)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi";
	input << "{\\fonttbl";
	input << "{\\f0\\fcharset77 Tester;}";
	input << "{\\f1\\fcharset186 Band-Maid;}";
	input << "{\\f2\\fcharset128 Tingling;}";
	input << "}}";

	RtfDomDocument* doc = parser.ParseDocument(&input);

	ASSERT_NE(nullptr, doc);

	EXPECT_STREQ("Tester", doc->GetFont(0).name.c_str());
	EXPECT_EQ(eRtfCharacterSet_AppleMacintoshRoman, doc->GetFont(0).character_set);

	EXPECT_STREQ("Band-Maid", doc->GetFont(1).name.c_str());
	EXPECT_EQ(eRtfCharacterSet_Baltic, doc->GetFont(1).character_set);

	EXPECT_STREQ("Tingling", doc->GetFont(2).name.c_str());
	EXPECT_EQ(eRtfCharacterSet_ShiftJis, doc->GetFont(2).character_set);
}

TEST(TextParserRtf, ColorTable)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi";
	input << "{\\colortbl;";
	input << "\\red0\\green0\\blue0;";
	input << "\\red128\\green128\\blue128;";
	input << "}}";

	RtfDomDocument* doc = parser.ParseDocument(&input);

	ASSERT_NE(nullptr, doc);

	RtfColor color0;
	EXPECT_TRUE(doc->TryGetColor(color0, 0));
	EXPECT_EQ(0, color0.r);
	EXPECT_EQ(0, color0.g);
	EXPECT_EQ(0, color0.b);
	EXPECT_EQ(255, color0.a);

	RtfColor color1;
	EXPECT_TRUE(doc->TryGetColor(color1, 1));
	EXPECT_EQ(128, color1.r);
	EXPECT_EQ(128, color1.g);
	EXPECT_EQ(128, color1.b);
	EXPECT_EQ(255, color1.a);
}

TEST(TextParserRtf, ControlTrailingWhitespace)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi{\\fonttbl\\f0 Comic Sans MS;}\\f0\\pard    }";

	RtfDomDocument* doc = parser.ParseDocument(&input);

	ASSERT_NE(nullptr, doc);

	RtfDomElement* root = doc->GetRootElement();

	EXPECT_STREQ("   ", root->InnerText.c_str());
}