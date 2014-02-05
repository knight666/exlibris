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

#include "RtfStyleSheet.h"

namespace ExLibris
{

	struct RtfStyleSheet::ParseState
	{
		ParseState()
			: sheet_started(false)
			, style(nullptr)
		{
		}

		bool sheet_started;
		RtfStyle* style;
	};

	RtfStyleSheet::RtfStyleSheet(RtfDomDocument& a_Document)
		: m_Document(a_Document)
		, m_State(new ParseState)
	{
	}

	RtfStyleSheet::~RtfStyleSheet()
	{
		for (std::map<int, RtfStyle*>::iterator style_it = m_Styles.begin(); style_it != m_Styles.end(); ++style_it)
		{
			delete style_it->second;
		}
		m_Styles.clear();

		delete m_State;
	}

	size_t RtfStyleSheet::GetStyleCount() const
	{
		return m_Styles.size();
	}

	RtfStyle* RtfStyleSheet::FindStyleByName(const std::string& a_Name) const
	{
		for (std::map<int, RtfStyle*>::const_iterator style_it = m_Styles.begin(); style_it != m_Styles.end(); ++style_it)
		{
			RtfStyle* style = style_it->second;

			if (style->GetName() == a_Name)
			{
				return style;
			}
		}
		
		return nullptr;
	}

	RtfStyle* RtfStyleSheet::GetStyle(int a_Index)
	{
		std::map<int, RtfStyle*>::iterator found = m_Styles.find(a_Index);
		if (found != m_Styles.end())
		{
			return found->second;
		}

		RtfStyle* style = new RtfStyle(*this, m_Document);
		
		m_Styles.insert(std::make_pair(a_Index, style));

		return style;
	}

	IRtfParseable::Result RtfStyleSheet::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		if (a_Token.value == "stylesheet")
		{
			m_State->sheet_started = true;
			m_State->style = nullptr;

			a_State.SetTarget(this);

			return eResult_Handled;
		}
		else if (a_Token.value == "s")
		{
			if (!m_State->sheet_started || a_Token.parameter < 0)
			{
				return eResult_Invalid;
			}

			m_State->style = GetStyle(a_Token.parameter);
		}
		else if (!m_State->sheet_started)
		{
			return eResult_Invalid;
		}

		Result result = eResult_Propagate;

		if (m_State->style != nullptr)
		{
			result = m_State->style->Parse(a_State, a_Token);
			if (result != eResult_Propagate)
			{
				return result;
			}
		}
			
		return result;
	}

	IRtfParseable::Result RtfStyleSheet::_ParseValue(RtfParserState& a_State, const RtfToken& a_Token)
	{
		if (!m_State->sheet_started || m_State->style == nullptr)
		{
			return eResult_Invalid;
		}

		return m_State->style->Parse(a_State, a_Token);
	}

}; // namespace ExLibris