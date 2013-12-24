#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

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

TEST(Tokenizer, EnableOption)
{
	Tokenizer tk(nullptr);

	tk.EnableOptions(Tokenizer::eOption_Identifiers);
	EXPECT_TRUE(tk.IsOptionEnabled(Tokenizer::eOption_Identifiers));
	EXPECT_FALSE(tk.IsOptionDisabled(Tokenizer::eOption_Identifiers));
}

TEST(Tokenizer, DisableOption)
{
	Tokenizer tk(nullptr);

	tk.DisableOptions(Tokenizer::eOption_Identifiers);
	EXPECT_TRUE(tk.IsOptionDisabled(Tokenizer::eOption_Identifiers));
	EXPECT_FALSE(tk.IsOptionEnabled(Tokenizer::eOption_Identifiers));
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

TEST(Tokenizer, ReadTokenAndClearInput)
{
	Tokenizer tk(nullptr);
	
	std::stringstream ss;
	ss << "testing";

	tk.SetInput(&ss);

	EXPECT_TOKEN(Token::eType_Identifier, "testing", 1, 1);

	tk.SetInput(nullptr);

	const Token& t = tk.GetCurrentToken();
	EXPECT_EQ(Token::eType_End, t.type);
	EXPECT_STREQ("", t.text.c_str());
	EXPECT_EQ(1, t.column);
	EXPECT_EQ(1, t.line);
}