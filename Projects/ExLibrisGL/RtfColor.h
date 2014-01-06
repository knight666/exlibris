#pragma once

namespace ExLibris
{

	struct RtfColor
	{

	public:

		RtfColor(int a_Red, int a_Green, int a_Blue, int a_Alpha = 255)
			: r(a_Red)
			, g(a_Green)
			, b(a_Blue)
			, a(a_Alpha)
		{
		}
		RtfColor()
			: r(0)
			, g(0)
			, b(0)
			, a(255)
		{
		}
		RtfColor(const RtfColor& a_Color)
			: r(a_Color.r)
			, g(a_Color.g)
			, b(a_Color.b)
			, a(a_Color.a)
		{
		}

		int r;
		int g;
		int b;
		int a;

	}; // struct RtfColor

}; // namespace ExLibris