#include "ExLibris.Test.PCH.h"

#include <TextParserRtf.h>

using namespace ExLibris;

TEST(TextParserRtf, HelloWorld)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi{\\fonttbl\\f0\\fswiss Helvetica;}\\f0\\pard\nHello World!\\par\n}";

	RtfDomDocument* doc = parser.ParseDocument(&input);

	EXPECT_EQ(0, parser.GetWarnings().size());
	EXPECT_EQ(0, parser.GetErrors().size());

	ASSERT_NE(nullptr, doc);

	RtfFontTable* ft = doc->GetFontTable();

	EXPECT_EQ(RtfFont::eFamilyType_Swiss, ft->GetFont(0)->family);
	EXPECT_STREQ("Helvetica", ft->GetFont(0)->name.c_str());

	RtfDomElement* root = doc->GetRootElement();

	EXPECT_EQ(eRtfCharacterSet_Ansi, root->TextFormat.character_set);
	EXPECT_STREQ("Hello World!", root->InnerText.c_str());

	EXPECT_EQ(1, root->GetChildrenCount());
}

TEST(TextParserRtf, ExtendedControlSkipUnknownGroup)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi";
	input << "{\\*\\zanzibar{\\new\\group\\please}This text should be ignored.}";
	input << "This text is totally rad.";
	input << "}";

	RtfDomDocument* doc = parser.ParseDocument(&input);

	EXPECT_EQ(0, parser.GetWarnings().size());
	EXPECT_EQ(0, parser.GetErrors().size());

	ASSERT_NE(nullptr, doc);

	RtfDomElement* root = doc->GetRootElement();

	EXPECT_EQ(0, root->GetChildrenCount());
	EXPECT_STREQ("This text is totally rad.", root->InnerText.c_str());
}

TEST(TextParserRtf, FontTableUseDefaultFont)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi\\deff0{\\fonttbl\\f0Robotica;}Bleep bloop.}";

	RtfDomDocument* doc = parser.ParseDocument(&input);

	EXPECT_EQ(0, parser.GetWarnings().size());
	EXPECT_EQ(0, parser.GetErrors().size());

	ASSERT_NE(nullptr, doc);

	ASSERT_NE(nullptr, doc->GetFontTable()->GetDefault());
	EXPECT_STREQ("Robotica", doc->GetFontTable()->GetDefault()->name.c_str());

	RtfDomElement* root = doc->GetRootElement();

	ASSERT_NE(nullptr, root->TextFormat.font);

	EXPECT_EQ(RtfFont::eFamilyType_Nil, root->TextFormat.font->family);
	EXPECT_STREQ("Robotica", root->TextFormat.font->name.c_str());
	EXPECT_STREQ("Bleep bloop.", root->InnerText.c_str());
}

TEST(TextParserRtf, FontTableSetPitch)
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

	EXPECT_EQ(0, parser.GetWarnings().size());
	EXPECT_EQ(0, parser.GetErrors().size());

	ASSERT_NE(nullptr, doc);

	RtfFontTable* ft = doc->GetFontTable();

	EXPECT_STREQ("Magnified", ft->GetFont(0)->name.c_str());
	EXPECT_EQ(RtfFont::ePitch_Fixed, ft->GetFont(0)->pitch);

	EXPECT_STREQ("Simplified", ft->GetFont(1)->name.c_str());
	EXPECT_EQ(RtfFont::ePitch_Default, ft->GetFont(1)->pitch);

	EXPECT_STREQ("Testified", ft->GetFont(2)->name.c_str());
	EXPECT_EQ(RtfFont::ePitch_Variable, ft->GetFont(2)->pitch);
}

TEST(TextParserRtf, FontTableSetCharacterSet)
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

	EXPECT_EQ(0, parser.GetWarnings().size());
	EXPECT_EQ(0, parser.GetErrors().size());

	ASSERT_NE(nullptr, doc);

	RtfFontTable* ft = doc->GetFontTable();

	EXPECT_STREQ("Tester", ft->GetFont(0)->name.c_str());
	EXPECT_EQ(eRtfCharacterSet_AppleMacintoshRoman, ft->GetFont(0)->character_set);

	EXPECT_STREQ("Band-Maid", ft->GetFont(1)->name.c_str());
	EXPECT_EQ(eRtfCharacterSet_Baltic, ft->GetFont(1)->character_set);

	EXPECT_STREQ("Tingling", ft->GetFont(2)->name.c_str());
	EXPECT_EQ(eRtfCharacterSet_ShiftJis, ft->GetFont(2)->character_set);
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

	EXPECT_EQ(0, parser.GetWarnings().size());
	EXPECT_EQ(0, parser.GetErrors().size());

	ASSERT_NE(nullptr, doc);

	RtfColor color0;
	EXPECT_TRUE(doc->GetColorTable()->TryGetColor(color0, 0));
	EXPECT_EQ(0, color0.r);
	EXPECT_EQ(0, color0.g);
	EXPECT_EQ(0, color0.b);
	EXPECT_EQ(255, color0.a);

	RtfColor color1;
	EXPECT_TRUE(doc->GetColorTable()->TryGetColor(color1, 1));
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

	EXPECT_EQ(0, parser.GetWarnings().size());
	EXPECT_EQ(0, parser.GetErrors().size());

	ASSERT_NE(nullptr, doc);

	RtfDomElement* root = doc->GetRootElement();

	EXPECT_STREQ("   ", root->InnerText.c_str());
}