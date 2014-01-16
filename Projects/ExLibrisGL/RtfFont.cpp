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

#include "ExLibrisGL.PCH.h"

#include "RtfFont.h"

namespace ExLibris
{

	RtfFont::RtfFont()
		: m_Family(eFamilyType_Nil)
		, m_Pitch(ePitch_Default)
		, m_CharacterSet(eRtfCharacterSet_Default)
	{

	}

	const std::string& RtfFont::GetName() const
	{
		return m_Name;
	}

	void RtfFont::SetName(const std::string& a_Name)
	{
		m_Name = a_Name;
	}

	RtfFont::FamilyType RtfFont::GetFamilyType() const
	{
		return m_Family;
	}

	void RtfFont::SetFamilyType(RtfFont::FamilyType a_Type)
	{
		m_Family = a_Type;
	}

	RtfFont::Pitch RtfFont::GetPitch() const
	{
		return m_Pitch;
	}

	void RtfFont::SetPitch(Pitch a_Pitch)
	{
		m_Pitch = a_Pitch;
	}

	RtfCharacterSet RtfFont::GetCharacterSet() const
	{
		return m_CharacterSet;
	}

	void RtfFont::SetCharacterSet(RtfCharacterSet a_CharacterSet)
	{
		m_CharacterSet = a_CharacterSet;
	}

}; // namespace ExLibris