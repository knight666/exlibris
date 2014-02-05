#include "ExLibris.Test.PCH.h"

#include <RtfFontFormat.h>

#include "Tools.Parseable.h"

using namespace ExLibris;
using namespace ExLibris::Rtf;

TEST(RtfFontFormat, Construct)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	EXPECT_EQ(nullptr, ff.GetLocale());
	EXPECT_EQ(nullptr, ff.GetLocaleEastAsian());
	EXPECT_FALSE(ff.IsBold());
	EXPECT_FALSE(ff.IsItalic());
	EXPECT_FALSE(ff.GetProofing());
}

TEST(RtfFontFormat, Reset)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	ff.Reset();

	EXPECT_EQ(nullptr, ff.GetLocale());
	EXPECT_EQ(nullptr, ff.GetLocaleEastAsian());
	EXPECT_FALSE(ff.IsBold());
	EXPECT_FALSE(ff.IsItalic());
	EXPECT_FALSE(ff.GetProofing());
}

TEST(RtfFontFormat, ParsePlain)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "plain";

	EXPECT_PARSE_HANDLED(ff.Parse(s, t));

	EXPECT_EQ(nullptr, ff.GetLocale());
	EXPECT_EQ(nullptr, ff.GetLocaleEastAsian());
	EXPECT_FALSE(ff.IsBold());
	EXPECT_FALSE(ff.IsItalic());
	EXPECT_FALSE(ff.GetProofing());
}

TEST(RtfFontFormat, ParseBoldOn)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "b";

	EXPECT_PARSE_HANDLED(ff.Parse(s, t));

	EXPECT_TRUE(ff.IsBold());
}

TEST(RtfFontFormat, ParseBoldOff)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "b";
	t.parameter = 0;

	EXPECT_PARSE_HANDLED(ff.Parse(s, t));

	EXPECT_FALSE(ff.IsBold());
}

TEST(RtfFontFormat, ParseBoldInvalidParameter)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "b";
	t.parameter = 12;

	EXPECT_PARSE_HANDLED(ff.Parse(s, t));

	EXPECT_TRUE(ff.IsBold());
}

TEST(RtfFontFormat, ParseItalicOn)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "i";

	EXPECT_PARSE_HANDLED(ff.Parse(s, t));

	EXPECT_TRUE(ff.IsItalic());
}

TEST(RtfFontFormat, ParseItalicOff)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "i";
	t.parameter = 0;

	EXPECT_PARSE_HANDLED(ff.Parse(s, t));

	EXPECT_FALSE(ff.IsItalic());
}

TEST(RtfFontFormat, ParseItalicInvalidParameter)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "i";
	t.parameter = -9;

	EXPECT_PARSE_HANDLED(ff.Parse(s, t));

	EXPECT_TRUE(ff.IsItalic());
}

TEST(RtfFontFormat, ParseLanguage)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "lang";
	t.parameter = 7169;

	EXPECT_PARSE_HANDLED(ff.Parse(s, t));

	ASSERT_NE(nullptr, ff.GetLocale());
	EXPECT_EQ(Rtf::eCountry_Tunisia, ff.GetLocale()->country);
	EXPECT_EQ(Rtf::eLanguage_Arabic, ff.GetLocale()->language);
	EXPECT_TRUE(ff.GetProofing());
}

TEST(RtfFontFormat, ParseLanguageUndefined)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "lang";
	t.parameter = 1024;

	EXPECT_PARSE_HANDLED(ff.Parse(s, t));

	ASSERT_NE(nullptr, ff.GetLocale());
	EXPECT_EQ(Rtf::eCountry_None, ff.GetLocale()->country);
	EXPECT_EQ(Rtf::eLanguage_None, ff.GetLocale()->language);
	EXPECT_FALSE(ff.GetProofing());
}

TEST(RtfFontFormat, ParseLanguageInvalidParameter)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "lang";
	t.parameter = 333;

	EXPECT_PARSE_INVALID(ff.Parse(s, t));
}

TEST(RtfFontFormat, ParseLanguageNoParameter)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "lang";

	EXPECT_PARSE_INVALID(ff.Parse(s, t));
}

TEST(RtfFontFormat, ParseLanguageNoWorld)
{
	RtfDomDocument d(nullptr);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "lang";
	t.parameter = 1050;

	EXPECT_PARSE_INVALID(ff.Parse(s, t));
}

TEST(RtfFontFormat, ParseLanguageEastAsian)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "langfe";
	t.parameter = 1041;

	EXPECT_PARSE_HANDLED(ff.Parse(s, t));

	ASSERT_NE(nullptr, ff.GetLocaleEastAsian());
	EXPECT_EQ(Rtf::eCountry_Japan, ff.GetLocaleEastAsian()->country);
	EXPECT_EQ(Rtf::eLanguage_Japanese, ff.GetLocaleEastAsian()->language);
}

TEST(RtfFontFormat, ParseLanguageEastAsianInvalidParameter)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "langfe";
	t.parameter = 9128;

	EXPECT_PARSE_INVALID(ff.Parse(s, t));
}

TEST(RtfFontFormat, ParseLanguageEastAsianNoParameter)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "langfe";

	EXPECT_PARSE_INVALID(ff.Parse(s, t));
}

TEST(RtfFontFormat, ParseLanguageEastAsianNoWorld)
{
	RtfDomDocument d(nullptr);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "langfe";
	t.parameter = 1031;

	EXPECT_PARSE_INVALID(ff.Parse(s, t));
}

TEST(RtfFontFormat, ParseProofingOff)
{
	RtfDomDocument d(nullptr);
	FontFormat ff(d);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "noproof";

	EXPECT_PARSE_HANDLED(ff.Parse(s, t));

	EXPECT_FALSE(ff.GetProofing());
}