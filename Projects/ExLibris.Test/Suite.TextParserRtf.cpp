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