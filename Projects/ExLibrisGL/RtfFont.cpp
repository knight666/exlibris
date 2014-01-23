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
		: m_FontTable(a_FontTable)
		, m_Family(Rtf::eFamilyType_Nil)
		, m_Pitch(Rtf::ePitch_Default)
		, m_CharacterSet(Rtf::eCharacterSet_Default)
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

	Rtf::FamilyType RtfFont::GetFamilyType() const
	{
		return m_Family;
	}

	void RtfFont::SetFamilyType(Rtf::FamilyType a_Type)
	{
		m_Family = a_Type;
	}

	Rtf::Pitch RtfFont::GetPitch() const
	{
		return m_Pitch;
	}

	void RtfFont::SetPitch(Rtf::Pitch a_Pitch)
	{
		m_Pitch = a_Pitch;
	}

	Rtf::CharacterSet RtfFont::GetCharacterSet() const
	{
		return m_CharacterSet;
	}

	void RtfFont::SetCharacterSet(Rtf::CharacterSet a_CharacterSet)
	{
		m_CharacterSet = a_CharacterSet;
	}

	IRtfParseable::Result RtfFont::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		// family type

		if (a_Token.value == "froman")
		{
			SetFamilyType(Rtf::eFamilyType_Roman);

			return eResult_Handled;
		}
		else if (a_Token.value == "fswiss")
		{
			SetFamilyType(Rtf::eFamilyType_Swiss);

			return eResult_Handled;
		}
		else if (a_Token.value == "fmodern")
		{
			SetFamilyType(Rtf::eFamilyType_Modern);

			return eResult_Handled;
		}
		else if (a_Token.value == "fscript")
		{
			SetFamilyType(Rtf::eFamilyType_Script);

			return eResult_Handled;
		}
		else if (a_Token.value == "fdecor")
		{
			SetFamilyType(Rtf::eFamilyType_Decor);

			return eResult_Handled;
		}
		else if (a_Token.value == "ftech")
		{
			SetFamilyType(Rtf::eFamilyType_Tech);

			return eResult_Handled;
		}
		else if (a_Token.value == "fbidi")
		{
			SetFamilyType(Rtf::eFamilyType_Bidi);

			return eResult_Handled;
		}
		else if (a_Token.value == "fnil")
		{
			SetFamilyType(Rtf::eFamilyType_Nil);

			return eResult_Handled;
		}

		// pitch

		else if (a_Token.value == "fprq")
		{
			if (a_Token.parameter < Rtf::ePitch_Default || a_Token.parameter > Rtf::ePitch_Variable)
			{
				return eResult_Invalid;
			}

			SetPitch((Rtf::Pitch)a_Token.parameter);

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
				CHARSET_CASE(0, Rtf::eCharacterSet_Ansi);
				CHARSET_CASE(1, Rtf::eCharacterSet_Default);
				CHARSET_CASE(2, Rtf::eCharacterSet_Symbol);
				CHARSET_CASE(77, Rtf::eCharacterSet_AppleMacintoshRoman);
				CHARSET_CASE(78, Rtf::eCharacterSet_AppleMacintoshShiftJis);
				CHARSET_CASE(79, Rtf::eCharacterSet_AppleMacintoshHangul);
				CHARSET_CASE(80, Rtf::eCharacterSet_AppleMacintoshGb2332);
				CHARSET_CASE(81, Rtf::eCharacterSet_AppleMacintoshBig5);
				CHARSET_CASE(82, Rtf::eCharacterSet_AppleMacintoshJohabOld);
				CHARSET_CASE(83, Rtf::eCharacterSet_AppleMacintoshHebrew);
				CHARSET_CASE(84, Rtf::eCharacterSet_AppleMacintoshArabic);
				CHARSET_CASE(85, Rtf::eCharacterSet_AppleMacintoshGreek);
				CHARSET_CASE(86, Rtf::eCharacterSet_AppleMacintoshTurkish);
				CHARSET_CASE(87, Rtf::eCharacterSet_AppleMacintoshThai);
				CHARSET_CASE(88, Rtf::eCharacterSet_AppleMacintoshEasternEuropean);
				CHARSET_CASE(89, Rtf::eCharacterSet_AppleMacintoshRussian);
				CHARSET_CASE(128, Rtf::eCharacterSet_ShiftJis);
				CHARSET_CASE(129, Rtf::eCharacterSet_Hangul);
				CHARSET_CASE(130, Rtf::eCharacterSet_Johab);
				CHARSET_CASE(134, Rtf::eCharacterSet_Gb2332);
				CHARSET_CASE(136, Rtf::eCharacterSet_Big5);
				CHARSET_CASE(161, Rtf::eCharacterSet_Greek);
				CHARSET_CASE(162, Rtf::eCharacterSet_Turkish);
				CHARSET_CASE(163, Rtf::eCharacterSet_Vietnamese);
				CHARSET_CASE(177, Rtf::eCharacterSet_Hebrew);
				CHARSET_CASE(178, Rtf::eCharacterSet_Arabic);
				CHARSET_CASE(179, Rtf::eCharacterSet_ArabicTraditionalOld);
				CHARSET_CASE(180, Rtf::eCharacterSet_ArabicUserOld);
				CHARSET_CASE(181, Rtf::eCharacterSet_HebrewUserOld);
				CHARSET_CASE(186, Rtf::eCharacterSet_Baltic);
				CHARSET_CASE(204, Rtf::eCharacterSet_Russian);
				CHARSET_CASE(222, Rtf::eCharacterSet_Thai);
				CHARSET_CASE(238, Rtf::eCharacterSet_EasternEuropean);
				CHARSET_CASE(254, Rtf::eCharacterSet_IbmPcCodePage437);
				CHARSET_CASE(255, Rtf::eCharacterSet_Oem);

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