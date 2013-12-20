#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerNumber, Read)
{
	std::stringstream ss;
	ss << "12.15";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "12.15", 1, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerNumber, ReadLeadingZero)
{
	std::stringstream ss;
	ss << "0.5551";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "0.5551", 1, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerNumber, ReadDotOnly)
{
	std::stringstream ss;
	ss << ".";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, ".", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(TokenizerNumber, ReadNegative)
{
	std::stringstream ss;
	ss << "-555.91";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "-", 1, 1);
	EXPECT_TOKEN(Token::eType_Number, "555.91", 2, 1);
	EXPECT_END_TOKEN(8, 1);
}

TEST(TokenizerNumber, ReadPositive)
{
	std::stringstream ss;
	ss << "+1.5";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "+", 1, 1);
	EXPECT_TOKEN(Token::eType_Number, "1.5", 2, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerNumber, ReadSpecifierAfterFloat)
{
	std::stringstream ss;
	ss << "12.76f";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "12.76f", 1, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerNumber, ReadUpperCaseSpecifierAfterFloat)
{
	std::stringstream ss;
	ss << "3.8888F";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "3.8888F", 1, 1);
	EXPECT_END_TOKEN(8, 1);
}

TEST(TokenizerNumber, ReadTextAfterFloat)
{
	std::stringstream ss;
	ss << "12.67food";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "12.67f", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "ood", 7, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(TokenizerNumber, ReadSpecifierInMiddleOfFloat)
{
	std::stringstream ss;
	ss << "15.97f112";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "15.97f", 1, 1);
	EXPECT_TOKEN(Token::eType_Integer, "112", 7, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(TokenizerNumber, ReadSpecifierAfterDot)
{
	std::stringstream ss;
	ss << "1.F";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "1.F", 1, 1);
	EXPECT_END_TOKEN(4, 1);
}

TEST(TokenizerNumber, ReadContained)
{
	std::stringstream ss;
	ss << "Price: $ 52.122 per pound";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Identifier, "Price", 1, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ":", 6, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 7, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "$", 8, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 9, 1);
	EXPECT_TOKEN(Token::eType_Number, "52.122", 10, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 16, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "per", 17, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 20, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "pound", 21, 1);
	EXPECT_END_TOKEN(26, 1);
}

TEST(TokenizerNumber, ReadDoubleDot)
{
	std::stringstream ss;
	ss << "20.0.115";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "20.0", 1, 1);
	EXPECT_TOKEN(Token::eType_Number, ".115", 5, 1);
	EXPECT_END_TOKEN(9, 1);
}

TEST(TokenizerNumber, ReadLeadingDot)
{
	std::stringstream ss;
	ss << ".129";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, ".129", 1, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerNumber, ReadTripleLeadingDot)
{
	std::stringstream ss;
	ss << "...1901";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, ".", 1, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 2, 1);
	EXPECT_TOKEN(Token::eType_Number, ".1901", 3, 1);
	EXPECT_END_TOKEN(8, 1);
}

TEST(TokenizerNumber, ReadTrailingDot)
{
	std::stringstream ss;
	ss << "1877.";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "1877", 1, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 5, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerNumber, ReadDoubleTrailingDot)
{
	std::stringstream ss;
	ss << "131..";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "131", 1, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 4, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 5, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerNumber, ReadDotInHexadecimal)
{
	std::stringstream ss;
	ss << "0x118.120";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0x118", 1, 1);
	EXPECT_TOKEN(Token::eType_Number, ".120", 6, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(TokenizerNumber, ReadInvalidHexadecimal)
{
	std::stringstream ss;
	ss << "0x.999";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "x", 2, 1);
	EXPECT_TOKEN(Token::eType_Number, ".999", 3, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerNumber, ReadDotInOctal)
{
	std::stringstream ss;
	ss << "0128.90";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Octal, "012", 1, 1);
	EXPECT_TOKEN(Token::eType_Number, "8.90", 4, 1);
	EXPECT_END_TOKEN(8, 1);
}