#pragma once

#include <IRtfParseable.h>

namespace ExLibris
{

#define RESULT_TO_STRING(_result) #_result

	static const char* s_ResultMap[IRtfParseable::eResult_Handled + 1] = {
		RESULT_TO_STRING(eResult_Invalid),
		RESULT_TO_STRING(eResult_Propagate),
		RESULT_TO_STRING(eResult_Handled)
	};

#undef RESULT_TO_STRING

	inline const char* ResultToString(IRtfParseable::Result a_Result)
	{
		if (a_Result < 0 || a_Result > IRtfParseable::eResult_Handled)
		{
			return "<Invalid>";
		}
		else
		{
			return s_ResultMap[a_Result];
		}
	}

	inline ::testing::AssertionResult CompareResult(
		const char* a_ExpressionLeft, const char* a_ExpressionRight,
		const IRtfParseable::Result& a_ResultLeft, const IRtfParseable::Result& a_ResultRight
	)
	{
		if (a_ResultLeft == a_ResultRight)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			return ::testing::AssertionFailure() 
				<< "Value of: " << a_ExpressionRight << std::endl
				<< "  Actual: " << ResultToString(a_ResultRight) << std::endl
				<< "Expected: " << ResultToString(a_ResultLeft);
		}
	}

#define EXPECT_PARSE_INVALID(_actual)   EXPECT_PRED_FORMAT2(CompareResult, IRtfParseable::eResult_Invalid, _actual)
#define EXPECT_PARSE_PROPAGATE(_actual) EXPECT_PRED_FORMAT2(CompareResult, IRtfParseable::eResult_Propagate, _actual)
#define EXPECT_PARSE_HANDLED(_actual)   EXPECT_PRED_FORMAT2(CompareResult, IRtfParseable::eResult_Handled, _actual)

}; // namespace ExLibris