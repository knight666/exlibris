#pragma once

#include "RtfCharacterSet.h"

namespace ExLibris
{

	class RtfFont
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

		RtfFont();

		const std::string& GetName() const;
		void SetName(const std::string& a_Name);

		FamilyType GetFamilyType() const;
		void SetFamilyType(FamilyType a_Type);

		Pitch GetPitch() const;
		void SetPitch(Pitch a_Pitch);

		RtfCharacterSet GetCharacterSet() const;
		void SetCharacterSet(RtfCharacterSet a_CharacterSet);

	private:

		std::string m_Name;
		FamilyType m_Family;
		Pitch m_Pitch;
		RtfCharacterSet m_CharacterSet;

	}; // struct RtfFont

}; // namespace ExLibris