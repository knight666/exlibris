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

#include "RtfFontTable.h"

namespace ExLibris
{

	RtfFont::RtfFont(RtfFontTable& a_FontTable)
		: IRtfParseable(&a_FontTable)
		, m_FontTable(a_FontTable)
		, m_Family(eFamilyType_Nil)
		, m_Pitch(ePitch_Default)
		, m_CharacterSet(eRtfCharacterSet_Default)
	{
	}

	RtfFontTable& RtfFont::GetFontTable() const
	{
		return m_FontTable;
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

	IRtfParseable::Result RtfFont::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		// family type

		if (a_Token.value == "froman")
		{
			SetFamilyType(RtfFont::eFamilyType_Roman);

			return eResult_Handled;
		}
		else if (a_Token.value == "fswiss")
		{
			SetFamilyType(RtfFont::eFamilyType_Swiss);

			return eResult_Handled;
		}
		else if (a_Token.value == "fmodern")
		{
			SetFamilyType(RtfFont::eFamilyType_Modern);

			return eResult_Handled;
		}
		else if (a_Token.value == "fscript")
		{
			SetFamilyType(RtfFont::eFamilyType_Script);

			return eResult_Handled;
		}
		else if (a_Token.value == "fdecor")
		{
			SetFamilyType(RtfFont::eFamilyType_Decor);

			return eResult_Handled;
		}
		else if (a_Token.value == "ftech")
		{
			SetFamilyType(RtfFont::eFamilyType_Tech);

			return eResult_Handled;
		}
		else if (a_Token.value == "fbidi")
		{
			SetFamilyType(RtfFont::eFamilyType_Bidi);

			return eResult_Handled;
		}
		else if (a_Token.value == "fnil")
		{
			SetFamilyType(RtfFont::eFamilyType_Nil);

			return eResult_Handled;
		}

		// pitch

		else if (a_Token.value == "fprq")
		{
			if (a_Token.parameter < ePitch_Default || a_Token.parameter > ePitch_Variable)
			{
				return eResult_Invalid;
			}

			SetPitch((Pitch)a_Token.parameter);

			return eResult_Handled;
		}

		// character set

		else if (a_Token.value == "fcharset")
		{
			if (a_Token.parameter < 0 || a_Token.parameter > 255)
			{
				return eResult_Invalid;
			}

#define CHARSET_CASE(_index, _value) case _index: { SetCharacterSet(_value); return eResult_Handled; } break;

			switch (a_Token.parameter)
			{
				CHARSET_CASE(0, eRtfCharacterSet_Ansi);
				CHARSET_CASE(1, eRtfCharacterSet_Default);
				CHARSET_CASE(2, eRtfCharacterSet_Symbol);
				CHARSET_CASE(77, eRtfCharacterSet_AppleMacintoshRoman);
				CHARSET_CASE(78, eRtfCharacterSet_AppleMacintoshShiftJis);
				CHARSET_CASE(79, eRtfCharacterSet_AppleMacintoshHangul);
				CHARSET_CASE(80, eRtfCharacterSet_AppleMacintoshGb2332);
				CHARSET_CASE(81, eRtfCharacterSet_AppleMacintoshBig5);
				CHARSET_CASE(82, eRtfCharacterSet_AppleMacintoshJohabOld);
				CHARSET_CASE(83, eRtfCharacterSet_AppleMacintoshHebrew);
				CHARSET_CASE(84, eRtfCharacterSet_AppleMacintoshArabic);
				CHARSET_CASE(85, eRtfCharacterSet_AppleMacintoshGreek);
				CHARSET_CASE(86, eRtfCharacterSet_AppleMacintoshTurkish);
				CHARSET_CASE(87, eRtfCharacterSet_AppleMacintoshThai);
				CHARSET_CASE(88, eRtfCharacterSet_AppleMacintoshEasternEuropean);
				CHARSET_CASE(89, eRtfCharacterSet_AppleMacintoshRussian);
				CHARSET_CASE(128, eRtfCharacterSet_ShiftJis);
				CHARSET_CASE(129, eRtfCharacterSet_Hangul);
				CHARSET_CASE(130, eRtfCharacterSet_Johab);
				CHARSET_CASE(134, eRtfCharacterSet_Gb2332);
				CHARSET_CASE(136, eRtfCharacterSet_Big5);
				CHARSET_CASE(161, eRtfCharacterSet_Greek);
				CHARSET_CASE(162, eRtfCharacterSet_Turkish);
				CHARSET_CASE(163, eRtfCharacterSet_Vietnamese);
				CHARSET_CASE(177, eRtfCharacterSet_Hebrew);
				CHARSET_CASE(178, eRtfCharacterSet_Arabic);
				CHARSET_CASE(179, eRtfCharacterSet_ArabicTraditionalOld);
				CHARSET_CASE(180, eRtfCharacterSet_ArabicUserOld);
				CHARSET_CASE(181, eRtfCharacterSet_HebrewUserOld);
				CHARSET_CASE(186, eRtfCharacterSet_Baltic);
				CHARSET_CASE(204, eRtfCharacterSet_Russian);
				CHARSET_CASE(222, eRtfCharacterSet_Thai);
				CHARSET_CASE(238, eRtfCharacterSet_EasternEuropean);
				CHARSET_CASE(254, eRtfCharacterSet_IbmPcCodePage437);
				CHARSET_CASE(255, eRtfCharacterSet_Oem);

			default:
				{
					return eResult_Invalid;

				} break;
			}

#undef CHARSET_CASE

		}

		else
		{
			return eResult_Propagate;
		}
	}

	IRtfParseable::Result RtfFont::_ParseValue(RtfParserState& a_State, const RtfToken& a_Token)
	{
		m_Name = a_Token.value;

		return eResult_Handled;
	}

}; // namespace ExLibris