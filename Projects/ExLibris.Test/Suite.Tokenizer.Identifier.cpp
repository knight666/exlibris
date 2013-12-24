#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerIdentifier, Read)
{
	std::stringstream ss;
	ss << "varAwesome";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Identifier, "varAwesome", 1, 1);
	EXPECT_END_TOKEN(11, 1);
}

TEST(TokenizerIdentifier, ReadVariableName)
{
	std::stringstream ss;
	ss << "data2";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Identifier, "data2", 1, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerIdentifier, ReadUnderscored)
{
	std::stringstream ss;
	ss << "snake_case_please";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Identifier, "snake_case_please", 1, 1);
	EXPECT_END_TOKEN(18, 1);
}

TEST(TokenizerIdentifier, ReadLeadingUnderscore)
{
	std::stringstream ss;
	ss << "__fifteen";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Identifier, "__fifteen", 1, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(TokenizerIdentifier, ReadTrailingUnderscore)
{
	std::stringstream ss;
	ss << "Underwhelmed_";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Identifier, "Underwhelmed_", 1, 1);
	EXPECT_END_TOKEN(14, 1);
}

TEST(TokenizerIdentifier, ReadVariableAndNumber)
{
	std::stringstream ss;
	ss << "version2.0.8";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Identifier, "version2", 1, 1);
	EXPECT_TOKEN(Token::eType_Number, ".0", 9, 1);
	EXPECT_TOKEN(Token::eType_Number, ".8", 11, 1);
	EXPECT_END_TOKEN(13, 1);
}