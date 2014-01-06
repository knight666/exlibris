#pragma once

#include "RtfCharacterSet.h"

namespace ExLibris
{
	struct RtfFont;
}

namespace ExLibris
{

	struct RtfTextFormat
	{

	public:

		RtfTextFormat()
			: character_set(eRtfCharacterSet_Invalid)
			, font(nullptr)
		{
		}

		RtfCharacterSet character_set;
		RtfFont* font;

	}; // struct RtfFormat

}; // namespace ExLibris