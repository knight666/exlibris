#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

using namespace ExLibris;

#define EXPECT_TOKEN(_type, _text, _column, _line) { \
	EXPECT_TRUE(tk.ReadToken()); \
	const Token& t = tk.GetCurrentToken(); \
	EXPECT_EQ(_type, t.type); \
	EXPECT_STREQ(_text, t.text.c_str()); \
	EXPECT_EQ(_column, t.column); \
	EXPECT_EQ(_line, t.line); \
}

TEST(Tokenizer, Construct)
{
	std::stringstream ss;
	ss << "I'm a token.";

	Tokenizer tk(&ss);
	EXPECT_TRUE(tk.IsNextTokenAvailable());
}

TEST(Tokenizer, ConstructEmpty)
{
	Tokenizer tk(nullptr);
	EXPECT_FALSE(tk.IsNextTokenAvailable());
}

TEST(Tokenizer, ConstructNoDataAvailable)
{
	std::stringstream ss;
	std::string s;
	ss >> s;

	Tokenizer tk(&ss);
	EXPECT_FALSE(tk.IsNextTokenAvailable());
}

TEST(Tokenizer, SetInput)
{
	Tokenizer tk(nullptr);

	std::stringstream ss;
	ss << "Magnificent.";

	tk.SetInput(&ss);

	EXPECT_TRUE(tk.IsNextTokenAvailable());
}

TEST(Tokenizer, SetInputNull)
{
	Tokenizer tk(nullptr);

	tk.SetInput(nullptr);

	EXPECT_FALSE(tk.IsNextTokenAvailable());
}

TEST(Tokenizer, SetInputNoDataAvailable)
{
	Tokenizer tk(nullptr);

	std::stringstream ss;
	std::string s;
	ss >> s;

	tk.SetInput(&ss);

	EXPECT_FALSE(tk.IsNextTokenAvailable());
}

TEST(Tokenizer, ReadString)
{
	std::stringstream ss;
	ss << "Teacup";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_String, "Teacup", 1, 1);
}

TEST(Tokenizer, ReadWhitespace)
{
	std::stringstream ss;
	ss << "    \t   \t   ";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, "    \t   \t   ", 1, 1);
}

TEST(Tokenizer, ReadSentence)
{
	std::stringstream ss;
	ss << "Mary had a bomb";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_String, "Mary", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 5, 1);
	EXPECT_TOKEN(Token::eType_String, "had", 6, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 9, 1);
	EXPECT_TOKEN(Token::eType_String, "a", 10, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 11, 1);
	EXPECT_TOKEN(Token::eType_String, "bomb", 12, 1);
}