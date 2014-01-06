#pragma once

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

	public:

		RtfFont()
			: family(eFamilyType_Nil)
		{
		}

		FamilyType family;
		std::string name;

	}; // struct RtfFont

}; // namespace ExLibris