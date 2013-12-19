#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerScientific, Read)
{
	std::stringstream ss;
	ss << "1.5e-6";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Scientific, "1.5e-6", 1, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerScientific, ReadFromInteger)
{
	std::stringstream ss;
	ss << "1e+12";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Scientific, "1e+12", 1, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerScientific, ReadFromOctal)
{
	std::stringstream ss;
	ss << "0e-0";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Scientific, "0e-0", 1, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerScientific, ReadNumberNoPower)
{
	std::stringstream ss;
	ss << "2.65e+";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "2.65", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "e", 5, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "+", 6, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerScientific, ReadNumberNoNaturalLog)
{
	std::stringstream ss;
	ss << "0.5e12";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Number, "0.5", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "e", 4, 1);
	EXPECT_TOKEN(Token::eType_Integer, "12", 5, 1);
	EXPECT_END_TOKEN(7, 1);
}