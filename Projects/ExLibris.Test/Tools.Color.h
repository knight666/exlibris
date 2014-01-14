#pragma once

#include <RtfColor.h>

namespace ExLibris
{

	inline std::string ColorToHtmlString(const RtfColor& a_Color)
	{
		char buffer[256] = { 0 };
		sprintf_s(buffer, 256, "#%02X%02X%02X", a_Color.r, a_Color.g, a_Color.b, a_Color.a);

		return buffer;
	}

	inline ::testing::AssertionResult CompareColorRgba(
		const char* a_ExpressionLeft, const char* a_ExpressionRight,
		const RtfColor& a_ColorLeft, const RtfColor& a_ColorRight
	)
	{
		if (
			a_ColorLeft.r == a_ColorRight.r &&
			a_ColorLeft.g == a_ColorRight.g &&
			a_ColorLeft.b == a_ColorRight.b &&
			a_ColorLeft.a == a_ColorRight.a
		)
		{
			return ::testing::AssertionSuccess();
		}

		::testing::AssertionResult result = ::testing::AssertionFailure();

		result << "Color: " << a_ExpressionRight << std::endl;

		result << "  Actual: "
			<< ColorToHtmlString(a_ColorRight)
			<< " (r: " << a_ColorRight.r
			<< ", g: " << a_ColorRight.g
			<< ", b: " << a_ColorRight.b
			<< ", a: " << a_ColorRight.a
			<< ")"
			<< std::endl;
		result << "Expected: "
			<< ColorToHtmlString(a_ColorLeft)
			<< " (r: " << a_ColorLeft.r
			<< ", g: " << a_ColorLeft.g
			<< ", b: " << a_ColorLeft.b
			<< ", a: " << a_ColorLeft.a
			<< ")"
			<< std::endl;

		return result;
	}

	inline ::testing::AssertionResult CompareColorRgb(
		const char* a_ExpressionLeft, const char* a_ExpressionRight,
		const RtfColor& a_ColorLeft, const RtfColor& a_ColorRight
	)
	{
		if (
			a_ColorLeft.r == a_ColorRight.r &&
			a_ColorLeft.g == a_ColorRight.g &&
			a_ColorLeft.b == a_ColorRight.b
		)
		{
			return ::testing::AssertionSuccess();
		}

		::testing::AssertionResult result = ::testing::AssertionFailure();

		result << "Color: " << a_ExpressionRight << std::endl;

		result << "  Actual: "
			<< ColorToHtmlString(a_ColorRight)
			<< " (r: " << a_ColorRight.r
			<< ", g: " << a_ColorRight.g
			<< ", b: " << a_ColorRight.b
			<< ", a: " << a_ColorRight.a
			<< ")"
			<< std::endl;
		result << "Expected: "
			<< ColorToHtmlString(a_ColorLeft)
			<< " (r: " << a_ColorLeft.r
			<< ", g: " << a_ColorLeft.g
			<< ", b: " << a_ColorLeft.b
			<< ", a: " << a_ColorLeft.a
			<< ")"
			<< std::endl;

		return result;
	}

#define EXPECT_COLOR_RGBA(_r, _g, _b, _a, _color) { \
	EXPECT_PRED_FORMAT2(CompareColorRgba, ::ExLibris::RtfColor(_r, _g, _b, _a), _color); \
}

#define EXPECT_COLOR_RGB(_r, _g, _b, _color) { \
	EXPECT_PRED_FORMAT2(CompareColorRgb, ::ExLibris::RtfColor(_r, _g, _b), _color); \
}

}; // namespace ExLibris