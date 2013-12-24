#include "ExLibris.Test.PCH.h"

#include <Tokenizer.h>

#include "Tools.Tokenizer.h"

using namespace ExLibris;

TEST(TokenizerText, ReadLowerCase)
{
	std::stringstream ss;
	ss << "c";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Text, "c", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(TokenizerText, ReadUpperCase)
{
	std::stringstream ss;
	ss << "D";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Text, "D", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(TokenizerText, ReadWord)
{
	std::stringstream ss;
	ss << "Teacup";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Text, "Teacup", 1, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerText, ReadContractedWord)
{
	std::stringstream ss;
	ss << "you'll'rn't";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Text, "you", 1, 1);
	EXPECT_TOKEN(Token::eType_String, "'ll'", 4, 1);
	EXPECT_TOKEN(Token::eType_Text, "rn", 8, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "'", 10, 1);
	EXPECT_TOKEN(Token::eType_Text, "t", 11, 1);
	EXPECT_END_TOKEN(12, 1);
}

TEST(TokenizerText, ReadUnderscored)
{
	std::stringstream ss;
	ss << "m_TestsValidated";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Text, "m", 1, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "_", 2, 1);
	EXPECT_TOKEN(Token::eType_Text, "TestsValidated", 3, 1);
	EXPECT_END_TOKEN(17, 1);
}

TEST(TokenizerText, ReadLeadingUnderscore)
{
	std::stringstream ss;
	ss << "_Bottleship";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Symbol, "_", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "Bottleship", 2, 1);
	EXPECT_END_TOKEN(12, 1);
}

TEST(TokenizerText, ReadTrailingUnderscore)
{
	std::stringstream ss;
	ss << "Ugh___";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Text, "Ugh", 1, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "_", 4, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "_", 5, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "_", 6, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerText, ReadSentence)
{
	std::stringstream ss;
	ss << "Mary had a bomb";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Text, "Mary", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 5, 1);
	EXPECT_TOKEN(Token::eType_Text, "had", 6, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 9, 1);
	EXPECT_TOKEN(Token::eType_Text, "a", 10, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 11, 1);
	EXPECT_TOKEN(Token::eType_Text, "bomb", 12, 1);
	EXPECT_END_TOKEN(16, 1);
}

TEST(TokenizerText, ReadInvalidOctal)
{
	std::stringstream ss;
	ss << "0BE88";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "BE", 2, 1);
	EXPECT_TOKEN(Token::eType_Integer, "88", 4, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(TokenizerText, ReadInvalidHexadecimalCharacters)
{
	std::stringstream ss;
	ss << "0xFAXE";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0xFA", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "XE", 5, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerText, ReadInvalidHexadecimalWrongSpecifier)
{
	std::stringstream ss;
	ss << "0X88E3";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "X", 2, 1);
	EXPECT_TOKEN(Token::eType_Integer, "88", 3, 1);
	EXPECT_TOKEN(Token::eType_Text, "E", 5, 1);
	EXPECT_TOKEN(Token::eType_Integer, "3", 6, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(TokenizerText, ReadInvalidHexadecimalNotEnoughValidCharacters)
{
	std::stringstream ss;
	ss << "0xir0";

	Tokenizer tk(&ss);
	tk.DisableOptions(Tokenizer::eOption_Identifiers);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "xir", 2, 1);
	EXPECT_TOKEN(Token::eType_Integer, "0", 5, 1);
	EXPECT_END_TOKEN(6, 1);
}