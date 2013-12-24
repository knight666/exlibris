#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerOctal, Read)
{
	std::stringstream ss;
	ss << "0777";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Octal, "0777", 1, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerOctal, ReadInvalid)
{
	std::stringstream ss;
	ss << "0F9GH";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "F9GH", 2, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerOctal, ReadNegative)
{
	std::stringstream ss;
	ss << "-0154";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "-", 1, 1);
	EXPECT_TOKEN(Token::eType_Octal, "0154", 2, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerOctal, ReadPositive)
{
	std::stringstream ss;
	ss << "+07";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "+", 1, 1);
	EXPECT_TOKEN(Token::eType_Octal, "07", 2, 1);
	EXPECT_END_TOKEN(4, 1);
}

TEST(TokenizerOctal, ReadSignedInvalid)
{
	std::stringstream ss;
	ss << "-0Buh";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "-", 1, 1);
	EXPECT_TOKEN(Token::eType_Integer, "0", 2, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "Buh", 3, 1);
	EXPECT_END_TOKEN(6, 1);
}