#include "ExLibris.Test.PCH.h"

#include <RtfTokenizer.h>

using namespace ExLibris;
using namespace ExLibris::Rtf;

#define TOKEN_TYPE_TO_STRING(_type) #_type

static const char* s_TokenTypeMap[RtfToken::eParseType_End + 1] = {
	TOKEN_TYPE_TO_STRING(eParseType_Invalid),
	TOKEN_TYPE_TO_STRING(eParseType_GroupOpen),
	TOKEN_TYPE_TO_STRING(eParseType_GroupClose),
	TOKEN_TYPE_TO_STRING(eParseType_Command),
	TOKEN_TYPE_TO_STRING(eParseType_CommandExtended),
	TOKEN_TYPE_TO_STRING(eParseType_Value),
	TOKEN_TYPE_TO_STRING(eParseType_Text),
	TOKEN_TYPE_TO_STRING(eParseType_End)
};

#undef TOKEN_TYPE_TO_STRING

inline const char* TokenTypeToString(RtfToken::ParseType a_Type)
{
	if (a_Type < 0 || a_Type > RtfToken::eParseType_End)
	{
		return "<Invalid>";
	}
	else
	{
		return s_TokenTypeMap[a_Type];
	}
}

inline ::testing::AssertionResult CompareToken(
	const char* a_ExpressionLeft, const char* a_ExpressionRight,
	const RtfToken& a_TokenLeft, const RtfToken& a_TokenRight
)
{
	if (
		a_TokenLeft.type == a_TokenRight.type && 
		a_TokenLeft.value == a_TokenRight.value &&
		a_TokenLeft.column == a_TokenRight.column &&
		a_TokenLeft.line == a_TokenRight.line
	)
	{
		return ::testing::AssertionSuccess();
	}

	return 
		::testing::AssertionFailure() << "Incorrect token." << std::endl
		<< " [ Actual ]" << std::endl
		<< "     Type: " << TokenTypeToString(a_TokenRight.type) << std::endl
		<< "    Value: " << a_TokenRight.value << std::endl
		<< " Position: " << "column: " << a_TokenRight.column << " line: " << a_TokenRight.line << std::endl
		<< std::endl
		<< "[ Expected ]" << std::endl
		<< "     Type: " << TokenTypeToString(a_TokenLeft.type) << std::endl
		<< "    Value: " << a_TokenLeft.value << std::endl
		<< " Position: " << "column: " << a_TokenLeft.column << " line: " << a_TokenLeft.line;
}

#define EXPECT_TEXT_TOKEN(_value, _column, _line) { \
	EXPECT_TRUE(tk.Read()); \
	const RtfToken& a = tk.GetCurrent(); \
	RtfToken e; \
	e.type = RtfToken::eParseType_Text; \
	e.value = _value; \
	e.column = _column; \
	e.line = _line; \
	EXPECT_PRED_FORMAT2(CompareToken, e, a); \
}

#define EXPECT_END_TOKEN(_column, _line) { \
	EXPECT_FALSE(tk.Read()); \
	const RtfToken& a = tk.GetCurrent(); \
	RtfToken e; \
	e.type = RtfToken::eParseType_End; \
	e.value = ""; \
	e.column = _column; \
	e.line = _line; \
	EXPECT_PRED_FORMAT2(CompareToken, e, a); \
}

TEST(RtfTokenizer, Construct)
{
	Tokenizer tk;

	EXPECT_FALSE(tk.IsNextAvailable());
	EXPECT_END_TOKEN(1, 1);
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

	EXPECT_TEXT_TOKEN("Hotdogs.", 1, 1);
	EXPECT_END_TOKEN(10, 1);
}

TEST(RtfTokenizer, ReadNoInput)
{
	Tokenizer tk;

	EXPECT_END_TOKEN(1, 1);
}

TEST(RtfTokenizer, ReadNoData)
{
	Tokenizer tk;

	std::stringstream ss;
	tk.SetInput(&ss);

	EXPECT_END_TOKEN(1, 1);
}