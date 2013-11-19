#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

using namespace ExLibris;

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

	EXPECT_TRUE(tk.ReadToken());

	const Token& t = tk.GetCurrentToken();

	EXPECT_EQ(Token::eType_String, t.type);
	EXPECT_STREQ("Teacup", t.text.c_str());
	EXPECT_EQ(0, t.column);
	EXPECT_EQ(0, t.line);
}

TEST(Tokenizer, ReadWhitespace)
{
	std::stringstream ss;
	ss << "    \t   \t   ";

	Tokenizer tk(&ss);

	EXPECT_TRUE(tk.ReadToken());

	const Token& t = tk.GetCurrentToken();

	EXPECT_EQ(Token::eType_Whitespace, t.type);
	EXPECT_STREQ("    \t   \t   ", t.text.c_str());
	EXPECT_EQ(0, t.column);
	EXPECT_EQ(0, t.line);
}

TEST(Tokenizer, ReadSentence)
{
	std::stringstream ss;
	ss << "Mary had a bomb";

	Tokenizer tk(&ss);

	const Token* t = nullptr;

	EXPECT_TRUE(tk.ReadToken());
	t = &tk.GetCurrentToken();
	EXPECT_EQ(Token::eType_String, t->type);
	EXPECT_STREQ("Mary", t->text.c_str());

	EXPECT_TRUE(tk.ReadToken());
	t = &tk.GetCurrentToken();
	EXPECT_EQ(Token::eType_Whitespace, t->type);
	EXPECT_STREQ(" ", t->text.c_str());

	EXPECT_TRUE(tk.ReadToken());
	t = &tk.GetCurrentToken();
	EXPECT_EQ(Token::eType_String, t->type);
	EXPECT_STREQ("had", t->text.c_str());

	EXPECT_TRUE(tk.ReadToken());
	t = &tk.GetCurrentToken();
	EXPECT_EQ(Token::eType_Whitespace, t->type);
	EXPECT_STREQ(" ", t->text.c_str());

	EXPECT_TRUE(tk.ReadToken());
	t = &tk.GetCurrentToken();
	EXPECT_EQ(Token::eType_String, t->type);
	EXPECT_STREQ("a", t->text.c_str());

	EXPECT_TRUE(tk.ReadToken());
	t = &tk.GetCurrentToken();
	EXPECT_EQ(Token::eType_Whitespace, t->type);
	EXPECT_STREQ(" ", t->text.c_str());

	EXPECT_TRUE(tk.ReadToken());
	t = &tk.GetCurrentToken();
	EXPECT_EQ(Token::eType_String, t->type);
	EXPECT_STREQ("bomb", t->text.c_str());
}