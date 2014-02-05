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