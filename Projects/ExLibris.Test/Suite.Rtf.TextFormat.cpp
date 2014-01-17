#include "ExLibris.Test.PCH.h"

#include <RtfTextFormat.h>

#include <RtfDomDocument.h>
#include <RtfFontTable.h>

#include "Tools.Color.h"

using namespace ExLibris;

TEST(RtfTextFormat, Construct)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	EXPECT_EQ(eRtfCharacterSet_Invalid, tf.GetCharacterSet());
	EXPECT_EQ(eRtfCharacterEncoding_SingleByteLowAnsi, tf.GetCharacterEncoding());
	EXPECT_EQ(nullptr, tf.GetFont());
	EXPECT_FLOAT_EQ(12.0f, tf.GetFontSize());
	EXPECT_EQ(nullptr, tf.GetLocale());
	ASSERT_NE(nullptr, tf.GetBackgroundColor());
	EXPECT_COLOR_RGB(0, 0, 0, *tf.GetBackgroundColor());
	ASSERT_NE(nullptr, tf.GetForegroundColor());
	EXPECT_COLOR_RGB(0, 0, 0, *tf.GetForegroundColor());
	EXPECT_TRUE(tf.IsKerningEnabled());
	EXPECT_EQ(1, tf.GetMinimumKerningSize());
	EXPECT_TRUE(tf.GetParagraphWidowControl());
}

TEST(RtfTextFormat, ParseCharacterSet)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "ansi";

	EXPECT_EQ(IRtfParseable::eResult_Handled, tf.Parse(s, t));

	EXPECT_EQ(eRtfCharacterSet_Ansi, tf.GetCharacterSet());
}

TEST(RtfTextFormat, ParseCharacterSetTwice)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "pc";
	EXPECT_EQ(IRtfParseable::eResult_Handled, tf.Parse(s, t));

	t.value = "pca";
	EXPECT_EQ(IRtfParseable::eResult_Invalid, tf.Parse(s, t));

	EXPECT_EQ(eRtfCharacterSet_IbmPcCodePage437, tf.GetCharacterSet());
}

TEST(RtfTextFormat, ParseFont)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "f";
	t.parameter = 12;

	EXPECT_EQ(IRtfParseable::eResult_Handled, tf.Parse(s, t));

	EXPECT_EQ(doc.GetFontTable()->GetFont(12), tf.GetFont());
}

TEST(RtfTextFormat, ParseFontInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "f";
	t.parameter = -1;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, tf.Parse(s, t));

	EXPECT_EQ(nullptr, tf.GetFont());
}

TEST(RtfTextFormat, ParseFontSize)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fs";
	t.parameter = 33;

	EXPECT_EQ(IRtfParseable::eResult_Handled, tf.Parse(s, t));

	EXPECT_EQ(16.5f, tf.GetFontSize());
}

TEST(RtfTextFormat, ParseFontSizeInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "fs";
	t.parameter = -16;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, tf.Parse(s, t));

	EXPECT_EQ(12.0f, tf.GetFontSize());
}

TEST(RtfTextFormat, ParseKerningEnabled)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "kerning";
	t.parameter = 1;

	EXPECT_EQ(IRtfParseable::eResult_Handled, tf.Parse(s, t));

	EXPECT_TRUE(tf.IsKerningEnabled());
}

TEST(RtfTextFormat, ParseKerningDisabled)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "kerning";
	t.parameter = 0;

	EXPECT_EQ(IRtfParseable::eResult_Handled, tf.Parse(s, t));

	EXPECT_FALSE(tf.IsKerningEnabled());
}

TEST(RtfTextFormat, ParseLocale)
{
	RtfWorld world;
	RtfDomDocument doc(&world);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "lang";
	t.parameter = 1061;

	EXPECT_EQ(IRtfParseable::eResult_Handled, tf.Parse(s, t));

	ASSERT_NE(nullptr, tf.GetLocale());
	EXPECT_EQ(eRtfCountry_Estonia, tf.GetLocale()->country);
	EXPECT_EQ(eRtfLanguage_Estonian, tf.GetLocale()->language);
}

TEST(RtfTextFormat, ParseLocaleNotFound)
{
	RtfWorld world;
	RtfDomDocument doc(&world);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "lang";
	t.parameter = 87712;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, tf.Parse(s, t));

	EXPECT_EQ(nullptr, tf.GetLocale());
}

TEST(RtfTextFormat, ParseLocaleInvalid)
{
	RtfWorld world;
	RtfDomDocument doc(&world);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "lang";
	t.parameter = -33;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, tf.Parse(s, t));

	EXPECT_EQ(nullptr, tf.GetLocale());
}

TEST(RtfTextFormat, ParseForegroundColor)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "cf";
	t.parameter = 22;

	EXPECT_EQ(IRtfParseable::eResult_Handled, tf.Parse(s, t));

	EXPECT_EQ(doc.GetColorTable()->GetColor(22), tf.GetForegroundColor());
}

TEST(RtfTextFormat, ParseForegroundColorInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "cf";
	t.parameter = -5;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, tf.Parse(s, t));

	EXPECT_EQ(doc.GetColorTable()->GetColor(0), tf.GetForegroundColor());
}

TEST(RtfTextFormat, ParseParagraphWidowControlDisabled)
{
	RtfDomDocument doc(nullptr);
	RtfTextFormat tf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "nowidctlpar";
	t.parameter = -1;

	EXPECT_EQ(IRtfParseable::eResult_Handled, tf.Parse(s, t));

	EXPECT_FALSE(tf.GetParagraphWidowControl());
}