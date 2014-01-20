/*
 * This file is a part of the ExLibris project.
 *
 * Copyright (C) 2013 Quinten Lansu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

#pragma once

#include "IRtfParseable.h"
#include "RtfCharacterSet.h"

namespace ExLibris
{
	class RtfFontTable;
}

namespace ExLibris
{

	class RtfFont
		: public IRtfParseable
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

		RtfFont(RtfFontTable& a_FontTable);

		RtfFontTable& GetFontTable() const;

		const std::string& GetName() const;
		void SetName(const std::string& a_Name);

		FamilyType GetFamilyType() const;
		void SetFamilyType(FamilyType a_Type);

		Pitch GetPitch() const;
		void SetPitch(Pitch a_Pitch);

		RtfCharacterSet GetCharacterSet() const;
		void SetCharacterSet(RtfCharacterSet a_CharacterSet);

	private:

		IRtfParseable::Result _ParseCommand(RtfParserState& a_State, const RtfToken& a_Token);
		IRtfParseable::Result _ParseValue(RtfParserState& a_State, const RtfToken& a_Token);

	private:

		RtfFontTable& m_FontTable;
		std::string m_Name;
		FamilyType m_Family;
		Pitch m_Pitch;
		RtfCharacterSet m_CharacterSet;

	}; // struct RtfFont

}; // namespace ExLibris