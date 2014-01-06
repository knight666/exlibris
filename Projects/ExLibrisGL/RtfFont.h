#pragma once

#include "RtfCharacterSet.h"

namespace ExLibris
{

	struct RtfFont
	{

	public:

		enum FamilyType
		{
			eFamilyType_Nil,
			eFamilyType_Roman,
			eFamilyType_Swiss,
			eFamilyType_Modern,
			eFamilyType_Script,
			eFamilyType_Decor,
			eFamilyType_Tech,
			eFamilyType_Bidi,
		};

		enum Pitch
		{
			ePitch_Default  = 0,
			ePitch_Fixed    = 1,
			ePitch_Variable = 2
		};

	public:

		RtfFont()
			: family(eFamilyType_Nil)
			, pitch(ePitch_Default)
			, character_set(eRtfCharacterSet_Default)
		{
		}

		std::string name;
		FamilyType family;
		Pitch pitch;
		RtfCharacterSet character_set;

	}; // struct RtfFont

}; // namespace ExLibris