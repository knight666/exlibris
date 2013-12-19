#pragma once

#include <Token.h>

namespace ExLibris
{

#define TOKEN_TYPE_TO_STRING(_type) #_type

	static const char* s_TokenTypeMap[Token::eType_End + 1] = {
		TOKEN_TYPE_TO_STRING(eType_Unprintable),
		TOKEN_TYPE_TO_STRING(eType_Text),
		TOKEN_TYPE_TO_STRING(eType_String),
		TOKEN_TYPE_TO_STRING(eType_Whitespace),
		TOKEN_TYPE_TO_STRING(eType_NewLine),
		TOKEN_TYPE_TO_STRING(eType_Symbol),
		TOKEN_TYPE_TO_STRING(eType_Integer),
		TOKEN_TYPE_TO_STRING(eType_Octal),
		TOKEN_TYPE_TO_STRING(eType_Hexadecimal),
		TOKEN_TYPE_TO_STRING(eType_Number),
		TOKEN_TYPE_TO_STRING(eType_End)
	};

#undef TOKEN_TYPE_TO_STRING

	inline const char* TokenTypeToString(Token::Type a_Type)
	{
		if (a_Type < 0 || a_Type > Token::eType_End)
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
		const Token& a_TokenLeft, const Token& a_TokenRight
	)
	{
		if (
			a_TokenLeft.type == a_TokenRight.type && 
			a_TokenLeft.text == a_TokenRight.text &&
			a_TokenLeft.column == a_TokenRight.column &&
			a_TokenLeft.line == a_TokenRight.line
			)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "Incorrect token." << std::endl;

			if (a_TokenLeft.type != a_TokenRight.type)
			{
				result
					<< std::endl
					<< "[Type]" << std::endl
					<< "   Actual:    " << TokenTypeToString(a_TokenRight.type) << std::endl
					<< " Expected:    " << TokenTypeToString(a_TokenLeft.type) << std::endl;
			}
			else
			{
				result
					<< std::endl
					<< "[Type]        " << TokenTypeToString(a_TokenLeft.type) << std::endl;
			}

			if (a_TokenLeft.text != a_TokenRight.text)
			{
				result
					<< std::endl
					<< "[Text]" << std::endl
					<< "   Actual:    \"" << a_TokenRight.text << "\"" << std::endl
					<< " Expected:    \"" << a_TokenLeft.text << "\"" << std::endl;
			}
			else
			{
				result
					<< std::endl
					<< "[Text]        \"" << a_TokenLeft.text << "\"" << std::endl;
			}

			if (a_TokenLeft.column != a_TokenRight.column || a_TokenLeft.line != a_TokenRight.line)
			{
				result
					<< std::endl
					<< "[Position]" << std::endl
					<< "   Actual:    line " << a_TokenRight.line << " column " << a_TokenRight.column << std::endl
					<< " Expected:    line " << a_TokenLeft.line << " column " << a_TokenLeft.column << std::endl;
			}
			else
			{
				result
					<< std::endl
					<< "[Position]    line " << a_TokenLeft.line << " column " << a_TokenLeft.column << std::endl;
			}

			return result;
		}
	}

#define EXPECT_TOKEN(_type, _text, _column, _line) { \
	EXPECT_TRUE(tk.ReadToken()); \
	const Token& a = tk.GetCurrentToken(); \
	Token e; \
	e.type = _type; \
	e.text = _text; \
	e.column = _column; \
	e.line = _line; \
	EXPECT_PRED_FORMAT2(CompareToken, e, a); \
}

#define EXPECT_END_TOKEN(_column, _line) { \
	EXPECT_FALSE(tk.ReadToken()); \
	const Token& a = tk.GetCurrentToken(); \
	Token e; \
	e.type = Token::eType_End; \
	e.text = ""; \
	e.column = _column; \
	e.line = _line; \
	EXPECT_PRED_FORMAT2(CompareToken, e, a); \
}

}; // namespace ExLibris