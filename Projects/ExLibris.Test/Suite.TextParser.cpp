#include "ExLibris.Test.PCH.h"

#include "Mock.TextParser.h"

using namespace ExLibris;

TEST(TextParser, Construct)
{
	MockTextParser parser;

	const TextParserToken& token = parser.GetToken();

	EXPECT_EQ(0, token.codepoint);
	EXPECT_EQ(TextParserToken::eChanged_None, token.changes);
	EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
	EXPECT_STREQ("", token.family_name.c_str());
	EXPECT_FLOAT_EQ(0.0f, token.size);
	EXPECT_EQ(eWeight_Normal, token.weight);
	EXPECT_EQ(eStyle_None, token.style);
	EXPECT_COLOR_EQ(0.0f, 0.0f, 0.0f, 1.0f, token.color);
	EXPECT_COLOR_EQ(1.0f, 1.0f, 1.0f, 1.0f, token.background_color);
}

TEST(TextParser, ReadCharacter)
{
	MockTextParser parser;
	parser.SetInput("C");

	EXPECT_TRUE(parser.ReadToken());

	const TextParserToken& token = parser.GetToken();

	EXPECT_EQ('C', token.codepoint);
	EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
}

TEST(TextParser, ReadWhitespace)
{
	MockTextParser parser;
	parser.SetInput(" \t");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(' ', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Whitespace, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('\t', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Whitespace, token.type);
	}
}

TEST(TextParser, ReadNewline)
{
	MockTextParser parser;
	parser.SetInput("\n");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('\n', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Newline, token.type);
	}
}

TEST(TextParser, ReadSentence)
{
	MockTextParser parser;
	parser.SetInput("I am\nan apple\r");

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('I', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(' ', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Whitespace, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('a', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('m', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('\n', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Newline, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('a', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('n', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ(' ', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Whitespace, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('a', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('p', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('p', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('l', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('e', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Character, token.type);
	}

	EXPECT_TRUE(parser.ReadToken());
	{
		const TextParserToken& token = parser.GetToken();

		EXPECT_EQ('\r', token.codepoint);
		EXPECT_EQ(TextParserToken::eCodepointType_Unprintable, token.type);
	}
}