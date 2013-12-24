#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerSymbol, Read)
{
	std::stringstream ss;
	ss << "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "!", 1, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "\"", 2, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "#", 3, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "$", 4, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "%", 5, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "&", 6, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "'", 7, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "(", 8, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ")", 9, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "*", 10, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "+", 11, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ",", 12, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "-", 13, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 14, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "/", 15, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ":", 16, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ";", 17, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "<", 18, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "=", 19, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ">", 20, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "?", 21, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "@", 22, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "[", 23, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "\\", 24, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "]", 25, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "^", 26, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "_", 27, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "`", 28, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "{", 29, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "|", 30, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "}", 31, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "~", 32, 1);
	EXPECT_END_TOKEN(33, 1);
}