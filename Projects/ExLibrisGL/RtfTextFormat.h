#pragma once

#include "RtfCharacterSet.h"

namespace ExLibris
{

	struct RtfTextFormat
	{

	public:

		RtfTextFormat()
			: character_set(eRtfCharacterSet_Invalid)
		{
		}

		RtfCharacterSet character_set;

	}; // struct RtfFormat

}; // namespace ExLibris