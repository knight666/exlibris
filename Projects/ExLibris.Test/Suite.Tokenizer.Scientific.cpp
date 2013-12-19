#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerScientific, Read)
{
	std::stringstream ss;
	ss << "1.5e-6";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Scientific, "1.5e-6", 1, 1);
	EXPECT_END_TOKEN(7, 1);
}