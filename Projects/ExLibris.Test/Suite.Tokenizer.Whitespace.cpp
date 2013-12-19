#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerWhitespace, ReadSpace)
{
	std::stringstream ss;
	ss << " ";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, " ", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(TokenizerWhitespace, ReadThreeSpaces)
{
	std::stringstream ss;
	ss << "   ";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, " ", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 2, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 3, 1);
	EXPECT_END_TOKEN(4, 1);
}

TEST(TokenizerWhitespace, ReadTab)
{
	std::stringstream ss;
	ss << "\t";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 1, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerWhitespace, ReadSpaceAndTab)
{
	std::stringstream ss;
	ss << " \t";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, " ", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 2, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerWhitespace, ReadTwoSpacesAndTab)
{
	std::stringstream ss;
	ss << "  \t";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, " ", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 2, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 3, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerWhitespace, ReadThreeSpacesAndTab)
{
	std::stringstream ss;
	ss << "   \t";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, " ", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 2, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 3, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 4, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerWhitespace, ReadFourSpacesAndTab)
{
	std::stringstream ss;
	ss << "    \t";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, " ", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 2, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 3, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 4, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 5, 1);
	EXPECT_END_TOKEN(9, 1);
}

TEST(TokenizerWhitespace, ReadTwoTabs)
{
	std::stringstream ss;
	ss << "\t\ts";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 5, 1);
	EXPECT_TOKEN(Token::eType_Text, "s", 9, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(TokenizerWhitespace, ReadTabWidthTwo)
{
	std::stringstream ss;
	ss << "\t \t\t";

	Tokenizer tk(&ss);
	tk.SetTabWidth(2);

	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 3, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 4, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 5, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerWhitespace, ReadTabWidthZero)
{
	std::stringstream ss;
	ss << " \t\t  ";

	Tokenizer tk(&ss);
	tk.SetTabWidth(0);

	EXPECT_TOKEN(Token::eType_Whitespace, " ", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 2, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 3, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 4, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 5, 1);
	EXPECT_END_TOKEN(6, 1);
}