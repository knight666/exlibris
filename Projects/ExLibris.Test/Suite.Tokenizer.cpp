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

TEST(Tokenizer, ReadText)
{
	std::stringstream ss;
	ss << "Teacup";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "Teacup", 1, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(Tokenizer, ReadWhitespace)
{
	std::stringstream ss;
	ss << "    \t   \t   ";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Whitespace, " ", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 2, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 3, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 4, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 5, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 6, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 7, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 8, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, "\t", 9, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 10, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 11, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 12, 1);
	EXPECT_END_TOKEN(13, 1);
}

TEST(Tokenizer, ReadSentence)
{
	std::stringstream ss;
	ss << "Mary had a bomb";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "Mary", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 5, 1);
	EXPECT_TOKEN(Token::eType_Text, "had", 6, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 9, 1);
	EXPECT_TOKEN(Token::eType_Text, "a", 10, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 11, 1);
	EXPECT_TOKEN(Token::eType_Text, "bomb", 12, 1);
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

TEST(Tokenizer, ReadQuotedString)
{
	std::stringstream ss;
	ss << "\"Tiles.\"";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_String, "\"Tiles.\"", 1, 1);
	EXPECT_END_TOKEN(9, 1);
}

TEST(Tokenizer, ReadQuotedStringWithOtherQuoteType)
{
	std::stringstream ss;
	ss << "'Spring\"aling\"'";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_String, "'Spring\"aling\"'", 1, 1);
	EXPECT_END_TOKEN(16, 1);
}

TEST(Tokenizer, ReadQuotedStringOpenEnded)
{
	std::stringstream ss;
	ss << "\"Start of something";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "\"", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "Start", 2, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 7, 1);
	EXPECT_TOKEN(Token::eType_Text, "of", 8, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 10, 1);
	EXPECT_TOKEN(Token::eType_Text, "something", 11, 1);
	EXPECT_END_TOKEN(20, 1);
}

TEST(Tokenizer, ReadQuotedStringMultipleLines)
{
	std::stringstream ss;
	ss << "Start 'a band\nthey said'.";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "Start", 1, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 6, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "'", 7, 1);
	EXPECT_TOKEN(Token::eType_Text, "a", 8, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 9, 1);
	EXPECT_TOKEN(Token::eType_Text, "band", 10, 1);
	EXPECT_TOKEN(Token::eType_NewLine, "\n", 14, 1);
	EXPECT_TOKEN(Token::eType_Text, "they", 1, 2);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 5, 2);
	EXPECT_TOKEN(Token::eType_Text, "said", 6, 2);
	EXPECT_TOKEN(Token::eType_Symbol, "'", 10, 2);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 11, 2);
	EXPECT_END_TOKEN(12, 2);
}

TEST(Tokenizer, ReadQuotedStringAndText)
{
	std::stringstream ss;
	ss << "\"That's what she said\", he said.";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_String, "\"That's what she said\"", 1, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ",", 23, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 24, 1);
	EXPECT_TOKEN(Token::eType_Text, "he", 25, 1);
	EXPECT_TOKEN(Token::eType_Whitespace, " ", 27, 1);
	EXPECT_TOKEN(Token::eType_Text, "said", 28, 1);
	EXPECT_TOKEN(Token::eType_Symbol, ".", 32, 1);
	EXPECT_END_TOKEN(33, 1);
}

TEST(Tokenizer, ReadQuotedStringSentence)
{
	std::stringstream ss;
	ss << "\"This is a drill, I think.\"";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_String, "\"This is a drill, I think.\"", 1, 1);
	EXPECT_END_TOKEN(28, 1);
}

TEST(Tokenizer, ReadNewLine)
{
	std::stringstream ss;
	ss << "worries\n";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "worries", 1, 1);
	EXPECT_TOKEN(Token::eType_NewLine, "\n", 8, 1);
	EXPECT_END_TOKEN(1, 2);
}

TEST(Tokenizer, ReadCarriageReturn)
{
	std::stringstream ss;
	ss << "buh\rneh?";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "buh", 1, 1);
	EXPECT_TOKEN(Token::eType_Unprintable, "\r", 4, 1);
	EXPECT_TOKEN(Token::eType_Text, "neh", 5, 1);
	EXPECT_TOKEN(Token::eType_Symbol, "?", 8, 1);
	EXPECT_END_TOKEN(9, 1);
}

TEST(Tokenizer, ReadSentenceOverSeveralLines)
{
	std::stringstream ss;
	ss << "THIS\nIS\r\nMEGABUNNY";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Text, "THIS", 1, 1);
	EXPECT_TOKEN(Token::eType_NewLine, "\n", 5, 1);
	EXPECT_TOKEN(Token::eType_Text, "IS", 1, 2);
	EXPECT_TOKEN(Token::eType_NewLine, "\r\n", 3, 2);
	EXPECT_TOKEN(Token::eType_Text, "MEGABUNNY", 1, 3);
	EXPECT_END_TOKEN(10, 3);
}

TEST(Tokenizer, ReadIntegerSingleDigit)
{
	std::stringstream ss;
	ss << "6";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "6", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(Tokenizer, ReadIntegerZero)
{
	std::stringstream ss;
	ss << "0";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_END_TOKEN(2, 1);
}

TEST(Tokenizer, ReadIntegerThreeDigits)
{
	std::stringstream ss;
	ss << "127";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "127", 1, 1);
	EXPECT_END_TOKEN(4, 1);
}

TEST(Tokenizer, ReadNegativeInteger)
{
	std::stringstream ss;
	ss << "-85";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "-", 1, 1);
	EXPECT_TOKEN(Token::eType_Integer, "85", 2, 1);
	EXPECT_END_TOKEN(4, 1);
}

TEST(Tokenizer, ReadPositiveInteger)
{
	std::stringstream ss;
	ss << "+166";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "+", 1, 1);
	EXPECT_TOKEN(Token::eType_Integer, "116", 2, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(Tokenizer, ReadOctal)
{
	std::stringstream ss;
	ss << "0777";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Octal, "0777", 1, 1);
	EXPECT_END_TOKEN(5, 1);
}

TEST(Tokenizer, ReadOctalInvalid)
{
	std::stringstream ss;
	ss << "0F9GH";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "F", 2, 1);
	EXPECT_TOKEN(Token::eType_Integer, "9", 3, 1);
	EXPECT_TOKEN(Token::eType_Text, "GH", 4, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(Tokenizer, ReadNegativeOctal)
{
	std::stringstream ss;
	ss << "-0154";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "-", 1, 1);
	EXPECT_TOKEN(Token::eType_Octal, "0154", 2, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(Tokenizer, ReadPositiveOctal)
{
	std::stringstream ss;
	ss << "+07";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "+", 1, 1);
	EXPECT_TOKEN(Token::eType_Octal, "07", 2, 1);
	EXPECT_END_TOKEN(4, 1);
}

TEST(Tokenizer, ReadSignedOctalInvalid)
{
	std::stringstream ss;
	ss << "-0Buh";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "-", 1, 1);
	EXPECT_TOKEN(Token::eType_Integer, "0", 2, 1);
	EXPECT_TOKEN(Token::eType_Text, "Buh", 3, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(Tokenizer, ReadHexadecimal)
{
	std::stringstream ss;
	ss << "0x1288f";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0x1288f", 1, 1);
	EXPECT_END_TOKEN(8, 1);
}

TEST(Tokenizer, ReadHexadecimalLowerCase)
{
	std::stringstream ss;
	ss << "0xabcdef";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0xabcdef", 1, 1);
	EXPECT_END_TOKEN(9, 1);
}

TEST(Tokenizer, ReadHexadecimalUpperCase)
{
	std::stringstream ss;
	ss << "0xBADFEC";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0xBADFEC", 1, 1);
	EXPECT_END_TOKEN(9, 1);
}

TEST(Tokenizer, ReadHexadecimalMixedCase)
{
	std::stringstream ss;
	ss << "0xAaABcDE";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0xAaABcDE", 1, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(Tokenizer, ReadHexadecimalInvalidCharacters)
{
	std::stringstream ss;
	ss << "0xA5EGOGO";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Hexadecimal, "0xA5E", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "GOGO", 6, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(Tokenizer, ReadHexadecimalWrongSpecifier)
{
	std::stringstream ss;
	ss << "0X771AABB";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "X", 2, 1);
	EXPECT_TOKEN(Token::eType_Integer, "771", 3, 1);
	EXPECT_TOKEN(Token::eType_Text, "AABB", 6, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(Tokenizer, ReadHexadecimalNotEnoughValidCharacters)
{
	std::stringstream ss;
	ss << "0xxxx0";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "xxxx", 2, 1);
	EXPECT_TOKEN(Token::eType_Integer, "0", 6, 1);
	EXPECT_END_TOKEN(7, 1);
}

TEST(Tokenizer, ReadHexadecimalNoData)
{
	std::stringstream ss;
	ss << "0x";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Integer, "0", 1, 1);
	EXPECT_TOKEN(Token::eType_Text, "x", 2, 1);
	EXPECT_END_TOKEN(3, 1);
}


TEST(Tokenizer, ReadHexadecimalNegative)
{
	std::stringstream ss;
	ss << "-0xDAD0";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "-", 1, 1);
	EXPECT_TOKEN(Token::eType_Hexadecimal, "0xDAD0", 2, 1);
	EXPECT_END_TOKEN(8, 1);
}

TEST(Tokenizer, ReadHexadecimalPositive)
{
	std::stringstream ss;
	ss << "+0x3A";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "+", 1, 1);
	EXPECT_TOKEN(Token::eType_Hexadecimal, "0x3A", 2, 1);
	EXPECT_END_TOKEN(6, 1);
}

TEST(Tokenizer, ReadSignedHexadecimalNoData)
{
	std::stringstream ss;
	ss << "+0x";

	Tokenizer tk(&ss);

	EXPECT_TOKEN(Token::eType_Symbol, "+", 1, 1);
	EXPECT_TOKEN(Token::eType_Integer, "0", 2, 1);
	EXPECT_TOKEN(Token::eType_Text, "x", 3, 1);
	EXPECT_END_TOKEN(4, 1);
}