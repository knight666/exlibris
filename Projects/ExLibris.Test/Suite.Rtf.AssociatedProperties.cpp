#include "ExLibris.Test.PCH.h"

#include <RtfAssociatedProperties.h>

#include <RtfDomDocument.h>
#include <RtfFontTable.h>

using namespace ExLibris;

TEST(RtfAssociatedProperties, Construct)
{
	RtfDomDocument doc(nullptr);
	RtfAssociatedProperties p(doc);

	EXPECT_EQ(nullptr, p.GetFont());
	EXPECT_FLOAT_EQ(12.0f, p.GetFontSize());
	EXPECT_EQ(nullptr, p.GetLocale());
	EXPECT_EQ(eRtfCharacterEncoding_SingleByteLowAnsi, p.GetCharacterEncoding());
	EXPECT_FALSE(p.IsBold());
	EXPECT_FALSE(p.IsItalic());
}

TEST(RtfAssociatedProperties, ParseFont)
{
	RtfDomDocument doc(nullptr);
	RtfAssociatedProperties p(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "af";
	t.parameter = 4;

	EXPECT_EQ(IRtfParseable::eResult_Handled, p.Parse(s, t));

	EXPECT_EQ(doc.GetFontTable()->GetFont(4), p.GetFont());
}

TEST(RtfAssociatedProperties, ParseFontInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfAssociatedProperties p(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "af";
	t.parameter = -12;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, p.Parse(s, t));

	EXPECT_EQ(nullptr, p.GetFont());
}

TEST(RtfAssociatedProperties, ParseFontSize)
{
	RtfDomDocument doc(nullptr);
	RtfAssociatedProperties p(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "afs";
	t.parameter = 29;

	EXPECT_EQ(IRtfParseable::eResult_Handled, p.Parse(s, t));

	EXPECT_FLOAT_EQ(14.5f, p.GetFontSize());
}

TEST(RtfAssociatedProperties, ParseFontSizeInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfAssociatedProperties p(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "afs";
	t.parameter = -98;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, p.Parse(s, t));

	EXPECT_FLOAT_EQ(12.0f, p.GetFontSize());
}

TEST(RtfAssociatedProperties, ParseLocale)
{
	RtfWorld world;
	RtfDomDocument doc(&world);
	RtfAssociatedProperties p(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "alang";
	t.parameter = 2055;

	EXPECT_EQ(IRtfParseable::eResult_Handled, p.Parse(s, t));

	ASSERT_NE(nullptr, p.GetLocale());
	EXPECT_EQ(Rtf::eCountry_Switzerland, p.GetLocale()->country);
	EXPECT_EQ(eRtfLanguage_German, p.GetLocale()->language);
}

TEST(RtfAssociatedProperties, ParseLocaleNotFound)
{
	RtfWorld world;
	RtfDomDocument doc(&world);
	RtfAssociatedProperties p(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "alang";
	t.parameter = 11788;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, p.Parse(s, t));

	EXPECT_EQ(nullptr, p.GetLocale());
}

TEST(RtfAssociatedProperties, ParseLocaleNoWorld)
{
	RtfDomDocument doc(nullptr);
	RtfAssociatedProperties p(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "alang";
	t.parameter = 1097;

	EXPECT_EQ(IRtfParseable::eResult_Invalid, p.Parse(s, t));

	EXPECT_EQ(nullptr, p.GetLocale());
}

TEST(RtfAssociatedProperties, ParseUnhandled)
{
	RtfDomDocument doc(nullptr);
	RtfAssociatedProperties p(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "f";
	t.parameter = 3;

	EXPECT_EQ(IRtfParseable::eResult_Propagate, p.Parse(s, t));
}