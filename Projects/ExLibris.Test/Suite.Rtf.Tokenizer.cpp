#include "ExLibris.Test.PCH.h"

#include <RtfTokenizer.h>

using namespace ExLibris::Rtf;

TEST(RtfTokenizer, Construct)
{
	Tokenizer tk;

	EXPECT_FALSE(tk.IsNextAvailable());
}

TEST(RtfTokenizer, IsNextAvailable)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "I'm a token.";

	tk.SetInput(&ss);

	EXPECT_TRUE(tk.IsNextAvailable());
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