#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerHexadecimal, Read)
{
	std::stringstream ss;
	ss << "0x1288f";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0x1288f", 1, 1);
	EXPECT_END_TOKEN(8, 1);
}

TEST(TokenizerHexadecimal, ReadLowerCase)
{
	std::stringstream ss;
	ss << "0xabcdef";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0xabcdef", 1, 1);
	EXPECT_END_TOKEN(9, 1);
}

TEST(TokenizerHexadecimal, ReadUpperCase)
{
	std::stringstream ss;
	ss << "0xBADFEC";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0xBADFEC", 1, 1);
	EXPECT_END_TOKEN(9, 1);
}

TEST(TokenizerHexadecimal, ReadMixedCase)
{
	std::stringstream ss;
	ss << "0xAaABcDE";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0xAaABcDE", 1, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(TokenizerHexadecimal, ReadInvalidCharacters)
{
	std::stringstream ss;
	ss << "0xA5EGOGO";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0xA5E", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "GOGO", 6, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(TokenizerHexadecimal, ReadWrongSpecifier)
{
	std::stringstream ss;
	ss << "0X771AABB";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "X", 2, 1);
	EXPECT_TOKEN(Token::eType_Integer, "771", 3, 1);
	EXPECT_TOKEN(Token::eType_Text, "AABB", 6, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(TokenizerHexadecimal, ReadNotEnoughValidCharacters)
{
	std::stringstream ss;
	ss << "0xxxx0";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "xxxx", 2, 1);
	EXPECT_TOKEN(Token::eType_Integer, "0", 6, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerHexadecimal, ReadNoData)
{
	std::stringstream ss;
	ss << "0x";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "x", 2, 1);
	EXPECT_END_TOKEN(3, 1);
}

TEST(TokenizerHexadecimal, ReadNegative)
{
	std::stringstream ss;
	ss << "-0xDAD0";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "-", 1, 1);
	EXPECT_TOKEN(Token::eType_Hexadecimal, "0xDAD0", 2, 1);
	EXPECT_END_TOKEN(8, 1);
}

TEST(TokenizerHexadecimal, ReadPositive)
{
	std::stringstream ss;
	ss << "+0x3A";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "+", 1, 1);
	EXPECT_TOKEN(Token::eType_Hexadecimal, "0x3A", 2, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerHexadecimal, ReadSignedNoData)
{
	std::stringstream ss;
	ss << "+0x";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "+", 1, 1);
	EXPECT_TOKEN(Token::eType_Integer, "0", 2, 1);
	EXPECT_TOKEN(Token::eType_Text, "x", 3, 1);
	EXPECT_END_TOKEN(4, 1);
}