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
#include "RtfTypes.h"

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

		RtfFont(RtfFontTable& a_FontTable);

		RtfFontTable& GetFontTable() const;

		const std::string& GetName() const;
		void SetName(const std::string& a_Name);

		Rtf::FamilyType GetFamilyType() const;
		void SetFamilyType(Rtf::FamilyType a_Type);

		Rtf::Pitch GetPitch() const;
		void SetPitch(Rtf::Pitch a_Pitch);

		Rtf::CharacterSet GetCharacterSet() const;
		void SetCharacterSet(Rtf::CharacterSet a_CharacterSet);

	private:

		IRtfParseable::Result _ParseCommand(RtfParserState& a_State, const RtfToken& a_Token);
		IRtfParseable::Result _ParseValue(RtfParserState& a_State, const RtfToken& a_Token);

	private:

		RtfFontTable& m_FontTable;
		std::string m_Name;
		Rtf::FamilyType m_Family;
		Rtf::Pitch m_Pitch;
		Rtf::CharacterSet m_CharacterSet;

	}; // struct RtfFont

}; // namespace ExLibris