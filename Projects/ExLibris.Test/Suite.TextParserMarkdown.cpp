#include "ExLibris.Test.PCH.h"

#include <TextParserMarkdown.h>

using namespace ExLibris;

TEST(TextParserMarkdown, Construct)
{
	TextParserMarkdown parser;

	const TextParserToken& token = parser.GetToken();

	EXPECT_EQ(0, token.codepoint);
	EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
	EXPECT_STREQ("", token.family_name.c_str());
	EXPECT_FLOAT_EQ(0.0f, token.size);
	EXPECT_EQ(eWeight_Normal, token.weight);
	EXPECT_EQ(eStyle_None, token.style);
	EXPECT_COLOR_EQ(0.0f, 0.0f, 0.0f, 1.0f, token.color);
	EXPECT_COLOR_EQ(1.0f, 1.0f, 1.0f, 1.0f, token.background_color);
}

TEST(TextParserMarkdown, ReadCodepoint)
{
	TextParserMarkdown parser;
	parser.SetInput("k");

	EXPECT_TRUE(parser.ReadToken());

	const TextParserToken& token = parser.GetToken();

	EXPECT_EQ('k', token.codepoint);
}

TEST(TextParserMarkdown, ReadThreeCodepoints)
{
	TextParserMarkdown parser;
	parser.SetInput("Bug");

	EXPECT_TRUE(parser.ReadToken());
	EXPECT_TRUE(parser.ReadToken());
	EXPECT_TRUE(parser.ReadToken());

	const TextParserToken& token = parser.GetToken();

	EXPECT_EQ('g', token.codepoint);
}

TEST(TextParserMarkdown, ReadCodepointEmpty)
{
	TextParserMarkdown parser;
	parser.SetInput("");

	EXPECT_FALSE(parser.ReadToken());

	const TextParserToken& token = parser.GetToken();

	EXPECT_EQ(0, token.codepoint);
}

TEST(TextParserMarkdown, ReadItalic)
{
	TextParserMarkdown parser;
	parser.SetInput("*");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}
}

TEST(TextParserMarkdown, ReadItalicCodepoint)
{
	TextParserMarkdown parser;
	parser.SetInput("*i");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('i', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}
}

TEST(TextParserMarkdown, ReadNormalAndItalicCodepoint)
{
	TextParserMarkdown parser;
	parser.SetInput("8*7");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('8', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eStyle_None, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('7', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}
}

TEST(TextParserMarkdown, ReadItalicAndNormalCodepoint)
{
	TextParserMarkdown parser;
	parser.SetInput("*H*M");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('H', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_None, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('M', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eStyle_None, token.style);
	}
}

TEST(TextParserMarkdown, ReadBold)
{
	TextParserMarkdown parser;
	parser.SetInput("**");

	EXPECT_TRUE(parser.ReadToken());

	const TextParserToken& token = parser.GetToken();

	EXPECT_EQ(0, token.codepoint);
	EXPECT_EQ(TextParserToken::eChanged_Weight, token.changes);
	EXPECT_EQ(eWeight_Bold, token.weight);
}

TEST(TextParserMarkdown, ReadBoldCodepoint)
{
	TextParserMarkdown parser;
	parser.SetInput("**b");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Weight, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('b', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
	}
}

TEST(TextParserMarkdown, ReadNormalAndBoldCodepoint)
{
	TextParserMarkdown parser;
	parser.SetInput("a**z");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('a', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eWeight_Normal, token.weight);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Weight, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('z', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
	}
}

TEST(TextParserMarkdown, ReadBoldAndNormalCodepoint)
{
	TextParserMarkdown parser;
	parser.SetInput("**p**r");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Weight, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('p', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Weight, token.changes);
		EXPECT_EQ(eWeight_Normal, token.weight);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('r', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eWeight_Normal, token.weight);
	}
}

TEST(TextParserMarkdown, ReadBoldItalicCodepoint)
{
	TextParserMarkdown parser;
	parser.SetInput("***S");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Weight, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('S', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}
}

TEST(TextParserMarkdown, ReadBoldItalicAndItalicCodepoint)
{
	TextParserMarkdown parser;
	parser.SetInput("***V**k");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Weight, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('V', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Weight, token.changes);
		EXPECT_EQ(eWeight_Normal, token.weight);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('k', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eWeight_Normal, token.weight);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}
}

TEST(TextParserMarkdown, ReadBoldItalicAndBoldCodepoint)
{
	TextParserMarkdown parser;
	parser.SetInput("***1*2");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Weight, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('1', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_None, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('2', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
		EXPECT_EQ(eStyle_None, token.style);
	}
}

TEST(TextParserMarkdown, Escape)
{
	TextParserMarkdown parser;
	parser.SetInput("\\");

	EXPECT_FALSE(parser.ReadToken());
}

TEST(TextParserMarkdown, EscapeAndSlash)
{
	TextParserMarkdown parser;
	parser.SetInput("\\\\");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('\\', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
	}
}

TEST(TextParserMarkdown, EscapeFiveSlashes)
{
	TextParserMarkdown parser;
	parser.SetInput("\\\\\\\\\\");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('\\', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('\\', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
	}

	EXPECT_FALSE(parser.ReadToken());
}

TEST(TextParserMarkdown, EscapeItalic)
{
	TextParserMarkdown parser;
	parser.SetInput("\\*");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('*', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
	}
}

TEST(TextParserMarkdown, EscapeItalicAndReadItalic)
{
	TextParserMarkdown parser;
	parser.SetInput("\\**");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('*', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}
}

TEST(TextParserMarkdown, EscapeItalicAndReadBold)
{
	TextParserMarkdown parser;
	parser.SetInput("\\***");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('*', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Weight, token.changes);
		EXPECT_EQ(eWeight_Bold, token.weight);
	}
}

TEST(TextParserMarkdown, EscapeItalicStar)
{
	TextParserMarkdown parser;
	parser.SetInput("*\\**");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_Italicized, token.style);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('*', token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(0, token.codepoint);
		EXPECT_EQ(TextParserToken::eChanged_Style, token.changes);
		EXPECT_EQ(eStyle_None, token.style);
	}
}