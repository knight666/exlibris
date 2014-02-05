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

	EXPECT_FALSE(ff.IsBold());
	EXPECT_FALSE(ff.IsItalic());
}

TEST(RtfFontFormat, Reset)
{
	RtfWorld w;
	RtfDomDocument d(&w);
	FontFormat ff(d);

	ff.Reset();

	EXPECT_FALSE(ff.IsBold());
	EXPECT_FALSE(ff.IsItalic());
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

	EXPECT_FALSE(ff.IsBold());
	EXPECT_FALSE(ff.IsItalic());
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