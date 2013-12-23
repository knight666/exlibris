#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerScientific, ReadNumber)
{
	std::stringstream ss;
	ss << "1.5e-6";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Scientific, "1.5e-6", 1, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerScientific, ReadNumberTwoSignifiers)
{
	std::stringstream ss;
	ss << "56.98EE-3";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "56.98", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "EE", 6, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "-", 8, 1);
	EXPECT_TOKEN(Token::eType_Integer, "3", 9, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(TokenizerScientific, ReadNumberNoPower)
{
	std::stringstream ss;
	ss << "2.65e+";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "2.65", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "e", 5, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "+", 6, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerScientific, ReadNumberNoSign)
{
	std::stringstream ss;
	ss << "0.5e12";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "0.5", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "e12", 4, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerScientific, ReadNumberNoPowerAndNoSign)
{
	std::stringstream ss;
	ss << "18.7e";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "18.7", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "e", 5, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerScientific, ReadNumberTrailingDot)
{
	std::stringstream ss;
	ss << "5.E+166";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "5", 1, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 2, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "E", 3, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "+", 4, 1);
	EXPECT_TOKEN(Token::eType_Integer, "166", 5, 1);
	EXPECT_END_TOKEN(8, 1);
}

TEST(TokenizerScientific, ReadInteger)
{
	std::stringstream ss;
	ss << "1e+12";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Scientific, "1e+12", 1, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerScientific, ReadIntegerNoPower)
{
	std::stringstream ss;
	ss << "777e+";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "777", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "e", 4, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "+", 5, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerScientific, ReadIntegerNoSign)
{
	std::stringstream ss;
	ss << "99e32";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "99", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "e32", 3, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerScientific, ReadIntegerNoPowerAndNoSign)
{
	std::stringstream ss;
	ss << "1e";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "1", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "e", 2, 1);
	EXPECT_END_TOKEN(3, 1);
}

TEST(TokenizerScientific, ReadIntegerThreeSignifiers)
{
	std::stringstream ss;
	ss << "118eEe";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "118", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "eEe", 4, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerScientific, ReadOctal)
{
	std::stringstream ss;
	ss << "01e-0";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Scientific, "01e-0", 1, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerScientific, ReadOctalNoPower)
{
	std::stringstream ss;
	ss << "0111e0134";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Octal, "0111", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "e0134", 5, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(TokenizerScientific, ReadOctalNoSign)
{
	std::stringstream ss;
	ss << "0127e15";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Octal, "0127", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "e15", 5, 1);
	EXPECT_END_TOKEN(8, 1);
}

TEST(TokenizerScientific, ReadOctalNoSignAndNoPower)
{
	std::stringstream ss;
	ss << "031e";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Octal, "031", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "e", 4, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerScientific, ReadOctalTwoSignifiers)
{
	std::stringstream ss;
	ss << "01ee";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Octal, "01", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "ee", 3, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerScientific, ReadOctalNoData)
{
	std::stringstream ss;
	ss << "0e89";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Identifier, "e89", 2, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerScientific, ReadHexadecimal)
{
	std::stringstream ss;
	ss << "0x91BCe-9";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0x91BCe", 1, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "-", 8, 1);
	EXPECT_TOKEN(Token::eType_Integer, "9", 9, 1);
	EXPECT_END_TOKEN(10, 1);
}