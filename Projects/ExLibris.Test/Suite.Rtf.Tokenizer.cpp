#include "ExLibris.Test.PCH.h"

#include <RtfTokenizer.h>

using namespace ExLibris;
using namespace ExLibris::Rtf;

TEST(RtfTokenizer, Construct)
{
	Tokenizer tk;

	EXPECT_FALSE(tk.IsNextAvailable());
	EXPECT_EQ(1, tk.GetCurrent().column);
	EXPECT_EQ(1, tk.GetCurrent().line);
	EXPECT_EQ(RtfToken::eParseType_Invalid, tk.GetCurrent().type);
	EXPECT_STREQ("", tk.GetCurrent().value.c_str());
}

TEST(RtfTokenizer, IsNextAvailable)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "#PCDATA";

	tk.SetInput(&ss);

	EXPECT_TRUE(tk.IsNextAvailable());
}

TEST(RtfTokenizer, IsNextAvailableNoInput)
{
	Tokenizer tk;
	tk.SetInput(nullptr);

	EXPECT_FALSE(tk.IsNextAvailable());
}

TEST(RtfTokenizer, IsNextAvailableNoData)
{
	Tokenizer tk;

	std::stringstream ss;
	std::string s;
	ss >> s;
	
	tk.SetInput(&ss);

	EXPECT_FALSE(tk.IsNextAvailable());
}

TEST(RtfTokenizer, ReadText)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "Hotdogs.";

	tk.SetInput(&ss);

	EXPECT_TRUE(tk.Read());

	EXPECT_EQ(1, tk.GetCurrent().column);
	EXPECT_EQ(1, tk.GetCurrent().line);
	EXPECT_EQ(RtfToken::eParseType_Text, tk.GetCurrent().type);
	EXPECT_STREQ("Hotdogs.", tk.GetCurrent().value.c_str());
}