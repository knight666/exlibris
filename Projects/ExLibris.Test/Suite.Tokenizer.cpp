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

#define EXPECT_END_TOKEN(_column, _line) { \
	EXPECT_FALSE(tk.ReadToken()); \
	const Token& t = tk.GetCurrentToken(); \
	EXPECT_EQ(Token::eType_End, t.type); \
	EXPECT_STREQ("", t.text.c_str()); \
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

TEST(Tokenizer, ReadEmpty)
{
	Tokenizer tk(nullptr);

	EXPECT_END_TOKEN(1, 1);
}

TEST(Tokenizer, ReadString)
{
	std::stringstream ss;
	ss << "Teacup";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_String, "Teacup", 1, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(Tokenizer, ReadWhitespace)
{
	std::stringstream ss;
	ss << "    \t   \t   ";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, "    \t   \t   ", 1, 1);
	EXPECT_END_TOKEN(13, 1);
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
	EXPECT_END_TOKEN(16, 1);
}

TEST(Tokenizer, ReadSymbols)
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

TEST(Tokenizer, ReadEncapsulatedSentence)
{
	std::stringstream ss;
	ss << "\"This is a drill, I think.\"";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "\"", 1, 1);
	EXPECT_TOKEN(Token::eType_String, "This", 2, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 6, 1);
	EXPECT_TOKEN(Token::eType_String, "is", 7, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 9, 1);
	EXPECT_TOKEN(Token::eType_String, "a", 10, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 11, 1);
	EXPECT_TOKEN(Token::eType_String, "drill", 12, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ",", 17, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 18, 1);
	EXPECT_TOKEN(Token::eType_String, "I", 19, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 20, 1);
	EXPECT_TOKEN(Token::eType_String, "think", 21, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 26, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "\"", 27, 1);
	EXPECT_END_TOKEN(28, 1);
}