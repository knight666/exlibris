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
		a_TokenLeft.parameter == a_TokenRight.parameter &&
		a_TokenLeft.group == a_TokenRight.group &&
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
		<< "Parameter: " << a_TokenRight.parameter << std::endl
		<< "    Group: " << a_TokenRight.group << std::endl
		<< " Position: " << "column: " << a_TokenRight.column << " line: " << a_TokenRight.line << std::endl
		<< std::endl
		<< "[ Expected ]" << std::endl
		<< "     Type: " << TokenTypeToString(a_TokenLeft.type) << std::endl
		<< "    Value: " << a_TokenLeft.value << std::endl
		<< "Parameter: " << a_TokenLeft.parameter << std::endl
		<< "    Group: " << a_TokenLeft.group << std::endl
		<< " Position: " << "column: " << a_TokenLeft.column << " line: " << a_TokenLeft.line;
}

#define EXPECT_INVALID_TOKEN(_value, _group, _column, _line) { \
	EXPECT_TRUE(tk.Read()); \
	const RtfToken& a = tk.GetCurrent(); \
	RtfToken e; \
	e.type = RtfToken::eParseType_Invalid; \
	e.value = _value; \
	e.parameter = -1; \
	e.group = 0; \
	e.column = _column; \
	e.line = _line; \
	EXPECT_PRED_FORMAT2(CompareToken, e, a); \
}

#define EXPECT_TEXT_TOKEN(_value, _group, _column, _line) { \
	EXPECT_TRUE(tk.Read()); \
	const RtfToken& a = tk.GetCurrent(); \
	RtfToken e; \
	e.type = RtfToken::eParseType_Text; \
	e.value = _value; \
	e.parameter = -1; \
	e.group = 0; \
	e.column = _column; \
	e.line = _line; \
	EXPECT_PRED_FORMAT2(CompareToken, e, a); \
}

#define EXPECT_GROUP_OPEN_TOKEN(_group, _column, _line) { \
	EXPECT_TRUE(tk.Read()); \
	const RtfToken& a = tk.GetCurrent(); \
	RtfToken e; \
	e.type = RtfToken::eParseType_GroupOpen; \
	e.value = ""; \
	e.parameter = -1; \
	e.group = _group; \
	e.column = _column; \
	e.line = _line; \
	EXPECT_PRED_FORMAT2(CompareToken, e, a); \
}

#define EXPECT_GROUP_CLOSE_TOKEN(_group, _column, _line) { \
	EXPECT_TRUE(tk.Read()); \
	const RtfToken& a = tk.GetCurrent(); \
	RtfToken e; \
	e.type = RtfToken::eParseType_GroupClose; \
	e.value = ""; \
	e.parameter = -1; \
	e.group = _group; \
	e.column = _column; \
	e.line = _line; \
	EXPECT_PRED_FORMAT2(CompareToken, e, a); \
}

#define EXPECT_COMMAND_TOKEN(_value, _parameter, _group, _column, _line) { \
	EXPECT_TRUE(tk.Read()); \
	const RtfToken& a = tk.GetCurrent(); \
	RtfToken e; \
	e.type = RtfToken::eParseType_Command; \
	e.value = _value; \
	e.parameter = _parameter; \
	e.group = _group; \
	e.column = _column; \
	e.line = _line; \
	EXPECT_PRED_FORMAT2(CompareToken, e, a); \
}

#define EXPECT_EXTENDED_COMMAND_TOKEN(_value, _parameter, _group, _column, _line) { \
	EXPECT_TRUE(tk.Read()); \
	const RtfToken& a = tk.GetCurrent(); \
	RtfToken e; \
	e.type = RtfToken::eParseType_CommandExtended; \
	e.value = _value; \
	e.parameter = _parameter; \
	e.group = _group; \
	e.column = _column; \
	e.line = _line; \
	EXPECT_PRED_FORMAT2(CompareToken, e, a); \
}

#define EXPECT_END_TOKEN(_group, _column, _line) { \
	EXPECT_FALSE(tk.Read()); \
	const RtfToken& a = tk.GetCurrent(); \
	RtfToken e; \
	e.type = RtfToken::eParseType_End; \
	e.value = ""; \
	e.parameter = -1; \
	e.group = _group; \
	e.column = _column; \
	e.line = _line; \
	EXPECT_PRED_FORMAT2(CompareToken, e, a); \
}

TEST(RtfTokenizer, Construct)
{
	Tokenizer tk;

	EXPECT_FALSE(tk.IsNextAvailable());
	EXPECT_END_TOKEN(0, 1, 1);
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

	EXPECT_TEXT_TOKEN("Hotdogs.", 0, 1, 1);
	EXPECT_END_TOKEN(0, 9, 1);
}

TEST(RtfTokenizer, ReadGroupOpen)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "{";

	tk.SetInput(&ss);

	EXPECT_GROUP_OPEN_TOKEN(1, 1, 1);
	EXPECT_END_TOKEN(1, 2, 1);
}

TEST(RtfTokenizer, ReadGroupClose)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "}";

	tk.SetInput(&ss);

	EXPECT_GROUP_CLOSE_TOKEN(-1, 1, 1);
	EXPECT_END_TOKEN(-1, 2, 1);
}

TEST(RtfTokenizer, ReadGroups)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "{{{}}{}}";

	tk.SetInput(&ss);

	EXPECT_GROUP_OPEN_TOKEN(1, 1, 1);
	EXPECT_GROUP_OPEN_TOKEN(2, 2, 1);
	EXPECT_GROUP_OPEN_TOKEN(3, 3, 1);
	EXPECT_GROUP_CLOSE_TOKEN(2, 4, 1);
	EXPECT_GROUP_CLOSE_TOKEN(1, 5, 1);
	EXPECT_GROUP_OPEN_TOKEN(2, 6, 1);
	EXPECT_GROUP_CLOSE_TOKEN(1, 7, 1);
	EXPECT_GROUP_CLOSE_TOKEN(0, 8, 1);
	EXPECT_END_TOKEN(0, 9, 1);
}

TEST(RtfTokenizer, ReadCommand)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "\\ansi";

	tk.SetInput(&ss);

	EXPECT_COMMAND_TOKEN("\\ansi", -1, 0, 1, 1);
	EXPECT_END_TOKEN(0, 6, 1);
}

TEST(RtfTokenizer, ReadCommandInvalid)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "\\0";

	tk.SetInput(&ss);

	EXPECT_INVALID_TOKEN("\\", 0, 1, 1);
	EXPECT_END_TOKEN(0, 3, 1);
}

TEST(RtfTokenizer, ReadCommandNoData)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "\\";

	tk.SetInput(&ss);

	EXPECT_INVALID_TOKEN("\\", 0, 1, 1);
	EXPECT_END_TOKEN(0, 2, 1);
}

TEST(RtfTokenizer, ReadCommandParameter)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "\\b1";

	tk.SetInput(&ss);

	EXPECT_COMMAND_TOKEN("\\b", 1, 0, 1, 1);
	EXPECT_END_TOKEN(0, 4, 1);
}

TEST(RtfTokenizer, ReadCommandParameterNegative)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "\\fs-228";

	tk.SetInput(&ss);

	EXPECT_COMMAND_TOKEN("\\fs", -228, 0, 1, 1);
	EXPECT_END_TOKEN(0, 8, 1);
}

TEST(RtfTokenizer, ReadExtendedCommand)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "\\*\\bloop";

	tk.SetInput(&ss);

	EXPECT_EXTENDED_COMMAND_TOKEN("\\*\\bloop", -1, 0, 1, 1);
	EXPECT_END_TOKEN(0, 9, 1);
}

TEST(RtfTokenizer, ReadExtendedCommandInvalid)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "\\*\\1";

	tk.SetInput(&ss);

	EXPECT_INVALID_TOKEN("\\*\\", 0, 1, 1);
	EXPECT_END_TOKEN(0, 5, 1);
}

TEST(RtfTokenizer, ReadExtendedCommandNotEnoughData)
{
	Tokenizer tk;

	std::stringstream ss;
	ss << "\\*";

	tk.SetInput(&ss);

	EXPECT_INVALID_TOKEN("\\*", 0, 1, 1);
	EXPECT_END_TOKEN(0, 3, 1);
}

TEST(RtfTokenizer, ReadNoInput)
{
	Tokenizer tk;

	EXPECT_END_TOKEN(0, 1, 1);
}

TEST(RtfTokenizer, ReadNoData)
{
	Tokenizer tk;

	std::stringstream ss;
	tk.SetInput(&ss);

	EXPECT_END_TOKEN(0, 1, 1);
}