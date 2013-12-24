#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerString, Read)
{
	std::stringstream ss;
	ss << "\"Tiles.\"";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_String, "\"Tiles.\"", 1, 1);
	EXPECT_END_TOKEN(9, 1);
}

TEST(TokenizerString, ReadWithOtherQuoteType)
{
	std::stringstream ss;
	ss << "'Spring\"aling\"'";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_String, "'Spring\"aling\"'", 1, 1);
	EXPECT_END_TOKEN(16, 1);
}

TEST(TokenizerString, ReadOpenEnded)
{
	std::stringstream ss;
	ss << "\"Start of something";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "\"", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "Start", 2, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 7, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "of", 8, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 10, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "something", 11, 1);
	EXPECT_END_TOKEN(20, 1);
}

TEST(TokenizerString, ReadMultipleLines)
{
	std::stringstream ss;
	ss << "Start 'a band\nthey said'.";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Identifier, "Start", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 6, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "'", 7, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "a", 8, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 9, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "band", 10, 1);
	EXPECT_TOKEN(Token::eType_NewLine, "\n", 14, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "they", 1, 2);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 5, 2);
	EXPECT_TOKEN(Token::eType_Identifier, "said", 6, 2);
	EXPECT_TOKEN(Token::eType_Symbol, "'", 10, 2);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 11, 2);
	EXPECT_END_TOKEN(12, 2);
}

TEST(TokenizerString, ReadUnprintableCharacters)
{
	std::stringstream ss;
	ss << "\"Listen to me: \ryou're not a bad person.\r\nNot in the slightest.\"";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "\"", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "Listen", 2, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 8, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "to", 9, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 11, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "me", 12, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ":", 14, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 15, 1);
	EXPECT_TOKEN(Token::eType_Unprintable, "\r", 16, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "you", 17, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "'", 20, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "re", 21, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 23, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "not", 24, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 27, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "a", 28, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 29, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "bad", 30, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 33, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "person", 34, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 40, 1);
	EXPECT_TOKEN(Token::eType_NewLine, "\r\n", 41, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "Not", 1, 2);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 4, 2);
	EXPECT_TOKEN(Token::eType_Identifier, "in", 5, 2);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 7, 2);
	EXPECT_TOKEN(Token::eType_Identifier, "the", 8, 2);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 11, 2);
	EXPECT_TOKEN(Token::eType_Identifier, "slightest", 12, 2);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 21, 2);
	EXPECT_TOKEN(Token::eType_Symbol, "\"", 22, 2);
	EXPECT_END_TOKEN(23, 2);
}

TEST(TokenizerString, ReadText)
{
	std::stringstream ss;
	ss << "\"That's what she said\", he said.";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_String, "\"That's what she said\"", 1, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ",", 23, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 24, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "he", 25, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 27, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "said", 28, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 32, 1);
	EXPECT_END_TOKEN(33, 1);
}

TEST(TokenizerString, ReadSentence)
{
	std::stringstream ss;
	ss << "\"This is a drill, I think.\"";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_String, "\"This is a drill, I think.\"", 1, 1);
	EXPECT_END_TOKEN(28, 1);
}