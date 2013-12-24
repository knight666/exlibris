#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerInteger, ReadSingleDigit)
{
	std::stringstream ss;
	ss << "6";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "6", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(TokenizerInteger, ReadZero)
{
	std::stringstream ss;
	ss << "0";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(TokenizerInteger, ReadThreeDigits)
{
	std::stringstream ss;
	ss << "127";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "127", 1, 1);
	EXPECT_END_TOKEN(4, 1);
}

TEST(TokenizerInteger, ReadNegative)
{
	std::stringstream ss;
	ss << "-85";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "-", 1, 1);
	EXPECT_TOKEN(Token::eType_Integer, "85", 2, 1);
	EXPECT_END_TOKEN(4, 1);
}

TEST(TokenizerInteger, ReadPositive)
{
	std::stringstream ss;
	ss << "+166";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "+", 1, 1);
	EXPECT_TOKEN(Token::eType_Integer, "166", 2, 1);
	EXPECT_END_TOKEN(5, 1);
}