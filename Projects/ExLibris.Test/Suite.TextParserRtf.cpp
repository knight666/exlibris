#include "ExLibris.Test.PCH.h"

#include <TextParserRtf.h>

#include "Tools.Color.h"

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

	EXPECT_EQ(Rtf::eFamilyType_Swiss, ft->GetFont(0)->GetFamilyType());
	EXPECT_STREQ("Helvetica", ft->GetFont(0)->GetName().c_str());

	RtfDomElement* root = doc->GetRootElement();

	EXPECT_EQ(Rtf::eCharacterSet_Ansi, root->GetTextFormat().GetCharacterSet());
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
	EXPECT_STREQ("Robotica", doc->GetFontTable()->GetDefault()->GetName().c_str());

	RtfDomElement* root = doc->GetRootElement();

	ASSERT_NE(nullptr, root->GetTextFormat().GetFont());

	EXPECT_EQ(Rtf::eFamilyType_Nil, root->GetTextFormat().GetFont()->GetFamilyType());
	EXPECT_STREQ("Robotica", root->GetTextFormat().GetFont()->GetName().c_str());
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

	EXPECT_STREQ("Magnified", ft->GetFont(0)->GetName().c_str());
	EXPECT_EQ(Rtf::ePitch_Fixed, ft->GetFont(0)->GetPitch());

	EXPECT_STREQ("Simplified", ft->GetFont(1)->GetName().c_str());
	EXPECT_EQ(Rtf::ePitch_Default, ft->GetFont(1)->GetPitch());

	EXPECT_STREQ("Testified", ft->GetFont(2)->GetName().c_str());
	EXPECT_EQ(Rtf::ePitch_Variable, ft->GetFont(2)->GetPitch());
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

	EXPECT_STREQ("Tester", ft->GetFont(0)->GetName().c_str());
	EXPECT_EQ(Rtf::eCharacterSet_AppleMacintoshRoman, ft->GetFont(0)->GetCharacterSet());

	EXPECT_STREQ("Band-Maid", ft->GetFont(1)->GetName().c_str());
	EXPECT_EQ(Rtf::eCharacterSet_Baltic, ft->GetFont(1)->GetCharacterSet());

	EXPECT_STREQ("Tingling", ft->GetFont(2)->GetName().c_str());
	EXPECT_EQ(Rtf::eCharacterSet_ShiftJis, ft->GetFont(2)->GetCharacterSet());
}

TEST(TextParserRtf, ColorTable)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi";
	input << "{\\colortbl;";
	input << "\\red65\\green12\\blue78;";
	input << "\\red128\\green128\\blue128;";
	input << "}}";

	RtfDomDocument* doc = parser.ParseDocument(&input);

	EXPECT_EQ(0, parser.GetWarnings().size());
	EXPECT_EQ(0, parser.GetErrors().size());

	ASSERT_NE(nullptr, doc);

	RtfColorTable* ct = doc->GetColorTable();

	EXPECT_COLOR_RGBA(0, 0, 0, 255, *ct->GetColor(0));
	EXPECT_COLOR_RGBA(65, 12, 78, 255, *ct->GetColor(1));
	EXPECT_COLOR_RGBA(128, 128, 128, 255, *ct->GetColor(2));
}

TEST(TextParserRtf, StyleSheet)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi";
	input << "{\\fonttbl{\\f0\\froman\\fprq2\\fcharset0 Times New Roman;}{\\f1\\froman\\fprq2\\fcharset2 Symbol;}}";
	input << "{\\colortbl;\\red65\\green12\\blue78;\\red128\\green128\\blue128;}";
	input << "{\\stylesheet{\\s0\\snext0\\cf1\\kerning1\\dbch\\af1\\langfe2052\\dbch\\afs32\\alang1081\\loch\\f0\\fs16\\lang1043 Standaard;}}";
	input << "}";

	RtfDomDocument* doc = parser.ParseDocument(&input);
	ASSERT_NE(nullptr, doc);

	RtfStyle* style = doc->GetStyleSheet()->GetStyle(0);

	ASSERT_NE(nullptr, style);
	EXPECT_STREQ("Standaard", style->GetName().c_str());

	RtfTextFormat& tf = style->GetTextFormat();
	EXPECT_EQ(Rtf::eCharacterSet_Ansi, tf.GetCharacterSet());
	EXPECT_EQ(Rtf::eCharacterEncoding_SingleByteLowAnsi, tf.GetCharacterEncoding());
	ASSERT_NE(nullptr, tf.GetLocale());
	EXPECT_EQ(Rtf::eCountry_Netherlands, tf.GetLocale()->country);
	EXPECT_EQ(Rtf::eLanguage_Dutch, tf.GetLocale()->language);
	ASSERT_NE(nullptr, tf.GetFont());
	EXPECT_STREQ("Times New Roman", tf.GetFont()->GetName().c_str());
	EXPECT_FLOAT_EQ(8.0f, tf.GetFontSize());
	ASSERT_NE(nullptr, tf.GetBackgroundColor());
	EXPECT_COLOR_RGB(0, 0, 0, *tf.GetBackgroundColor());
	ASSERT_NE(nullptr, tf.GetForegroundColor());
	EXPECT_COLOR_RGB(65, 12, 78, *tf.GetForegroundColor());
	EXPECT_TRUE(tf.GetParagraphWidowControl());
	EXPECT_TRUE(tf.IsKerningEnabled());
	EXPECT_EQ(1, tf.GetMinimumKerningSize());

	RtfAssociatedProperties p0 = style->GetCombinedPropertiesForCharacterEncoding(Rtf::eCharacterEncoding_SingleByteLowAnsi);
	EXPECT_EQ(Rtf::eCharacterEncoding_SingleByteLowAnsi, p0.GetCharacterEncoding());
	ASSERT_NE(nullptr, p0.GetFont());
	EXPECT_STREQ("Times New Roman", p0.GetFont()->GetName().c_str());
	EXPECT_FLOAT_EQ(8.0f, p0.GetFontSize());
	ASSERT_NE(nullptr, p0.GetLocale());
	EXPECT_EQ(Rtf::eCountry_Netherlands, p0.GetLocale()->country);
	EXPECT_EQ(Rtf::eLanguage_Dutch, p0.GetLocale()->language);

	RtfAssociatedProperties p1 = style->GetCombinedPropertiesForCharacterEncoding(Rtf::eCharacterEncoding_SingleByteHighAnsi);
	EXPECT_EQ(Rtf::eCharacterEncoding_SingleByteHighAnsi, p1.GetCharacterEncoding());
	ASSERT_NE(nullptr, p1.GetFont());
	EXPECT_STREQ("Times New Roman", p1.GetFont()->GetName().c_str());
	EXPECT_FLOAT_EQ(8.0f, p1.GetFontSize());
	ASSERT_NE(nullptr, p1.GetLocale());
	EXPECT_EQ(Rtf::eCountry_Netherlands, p1.GetLocale()->country);
	EXPECT_EQ(Rtf::eLanguage_Dutch, p1.GetLocale()->language);

	RtfAssociatedProperties p2 = style->GetCombinedPropertiesForCharacterEncoding(Rtf::eCharacterEncoding_DoubleByte);
	EXPECT_EQ(Rtf::eCharacterEncoding_DoubleByte, p2.GetCharacterEncoding());
	ASSERT_NE(nullptr, p2.GetFont());
	EXPECT_STREQ("Symbol", p2.GetFont()->GetName().c_str());
	EXPECT_FLOAT_EQ(16.0f, p2.GetFontSize());
	ASSERT_NE(nullptr, p2.GetLocale());
	EXPECT_EQ(Rtf::eCountry_India, p2.GetLocale()->country);
	EXPECT_EQ(Rtf::eLanguage_Hindi, p2.GetLocale()->language);
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