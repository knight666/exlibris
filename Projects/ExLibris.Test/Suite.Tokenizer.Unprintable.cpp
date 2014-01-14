#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerUnprintable, Read)
{
	std::stringstream ss;
	ss << "\16";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Unprintable, "\16", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(TokenizerUnprintable, ReadCarriageReturn)
{
	std::stringstream ss;
	ss << "\r";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Unprintable, "\r", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(TokenizerUnprintable, ReadCarriageReturnAndNewLine)
{
	std::stringstream ss;
	ss << "\r\n";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_NewLine, "\r\n", 1, 1);
	EXPECT_END_TOKEN(1, 2);
}

TEST(TokenizerUnprintable, ReadTab)
{
	std::stringstream ss;
	ss << "\t";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 1, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(TokenizerUnprintable, ReadEndOfData)
{
	std::stringstream ss;
	ss << "\0";

	Tokenizer tk(&ss);

	EXPECT_END_TOKEN(1, 1);
}