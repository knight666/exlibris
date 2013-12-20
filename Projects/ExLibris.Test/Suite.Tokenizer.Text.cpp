#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerText, ReadLowerCase)
{
	std::stringstream ss;
	ss << "c";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "c", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(TokenizerText, ReadUpperCase)
{
	std::stringstream ss;
	ss << "D";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "D", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(TokenizerText, ReadWord)
{
	std::stringstream ss;
	ss << "Teacup";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "Teacup", 1, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerText, ReadContractedWord)
{
	std::stringstream ss;
	ss << "you'll'rn't";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "you", 1, 1);
	EXPECT_TOKEN(Token::eType_String, "'ll'", 4, 1);
	EXPECT_TOKEN(Token::eType_Text, "rn", 8, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "'", 10, 1);
	EXPECT_TOKEN(Token::eType_Text, "t", 11, 1);
	EXPECT_END_TOKEN(12, 1);
}

TEST(Tokenizer, ReadSentence)
{
	std::stringstream ss;
	ss << "Mary had a bomb";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "Mary", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 5, 1);
	EXPECT_TOKEN(Token::eType_Text, "had", 6, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 9, 1);
	EXPECT_TOKEN(Token::eType_Text, "a", 10, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 11, 1);
	EXPECT_TOKEN(Token::eType_Text, "bomb", 12, 1);
	EXPECT_END_TOKEN(16, 1);
}