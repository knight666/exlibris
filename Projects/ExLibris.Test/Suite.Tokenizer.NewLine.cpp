#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerNewLine, Read)
{
	std::stringstream ss;
	ss << "worries\n";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "worries", 1, 1);
	EXPECT_TOKEN(Token::eType_NewLine, "\n", 8, 1);
	EXPECT_END_TOKEN(1, 2);
}

TEST(TokenizerNewLine, ReadCarriageReturn)
{
	std::stringstream ss;
	ss << "buh\rneh?";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "buh", 1, 1);
	EXPECT_TOKEN(Token::eType_Unprintable, "\r", 4, 1);
	EXPECT_TOKEN(Token::eType_Text, "neh", 5, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "?", 8, 1);
	EXPECT_END_TOKEN(9, 1);
}

TEST(TokenizerNewLine, ReadCarriageReturnAndNewLine)
{
	std::stringstream ss;
	ss << "aww\r\nyiss";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "aww", 1, 1);
	EXPECT_TOKEN(Token::eType_NewLine, "\r\n", 4, 1);
	EXPECT_TOKEN(Token::eType_Text, "yiss", 1, 2);
	EXPECT_END_TOKEN(5, 2);
}

TEST(TokenizerNewLine, ReadMultipleCarriageReturnsAndNewLines)
{
	std::stringstream ss;
	ss << "\r\r\n\n\r\n\r\n\r\r";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Unprintable, "\r", 1, 1);
	EXPECT_TOKEN(Token::eType_NewLine, "\r\n", 2, 1);
	EXPECT_TOKEN(Token::eType_NewLine, "\n", 1, 2);
	EXPECT_TOKEN(Token::eType_NewLine, "\r\n", 1, 3);
	EXPECT_TOKEN(Token::eType_NewLine, "\r\n", 1, 4);
	EXPECT_TOKEN(Token::eType_Unprintable, "\r", 1, 5);
	EXPECT_TOKEN(Token::eType_Unprintable, "\r", 2, 5);
	EXPECT_END_TOKEN(3, 5);
}

TEST(TokenizerNewLine, ReadSentenceOverSeveralLines)
{
	std::stringstream ss;
	ss << "THIS\nIS\r\nMEGABUNNY";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "THIS", 1, 1);
	EXPECT_TOKEN(Token::eType_NewLine, "\n", 5, 1);
	EXPECT_TOKEN(Token::eType_Text, "IS", 1, 2);
	EXPECT_TOKEN(Token::eType_NewLine, "\r\n", 3, 2);
	EXPECT_TOKEN(Token::eType_Text, "MEGABUNNY", 1, 3);
	EXPECT_END_TOKEN(10, 3);
}