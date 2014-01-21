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

#include "RtfFontTable.h"

#include "RtfDomDocument.h"

namespace ExLibris
{

	struct RtfFontTable::ParseState
	{
		ParseState()
			: group_parent(nullptr)
			, group_container(nullptr)
			, table_started(false)
		{
		}

		RtfParserGroup* group_parent;
		RtfParserGroup* group_container;
		bool table_started;
	};

	RtfFontTable::RtfFontTable(RtfDomDocument& a_Document)
		: IRtfParseable(&a_Document)
		, m_Document(a_Document)
		, m_Default(nullptr)
		, m_State(new ParseState)
	{
	}

	RtfFontTable::~RtfFontTable()
	{
		for (std::map<int, RtfFont*>::iterator font_it = m_Fonts.begin(); font_it != m_Fonts.end(); ++font_it)
		{
			delete font_it->second;
		}
		m_Fonts.clear();

		delete m_State;
	}

	size_t RtfFontTable::GetFontCount() const
	{
		return m_Fonts.size();
	}

	RtfFont* RtfFontTable::GetFont(int a_Index)
	{
		if (a_Index < 0)
		{
			return nullptr;
		}

		RtfFont* font = nullptr;

		std::map<int, RtfFont*>::iterator found = m_Fonts.find(a_Index);
		if (found != m_Fonts.end())
		{
			font = found->second;
		}
		else
		{
			font = new RtfFont(*this);

			m_Fonts.insert(std::make_pair(a_Index, font));
		}

		return font;
	}

	RtfFont* RtfFontTable::GetDefault() const
	{
		return m_Default;
	}

	void RtfFontTable::SetDefault(int a_Index)
	{
		RtfFont* font = GetFont(a_Index);
		if (font != nullptr)
		{
			m_Default = font;
		}
	}

	IRtfParseable::Result RtfFontTable::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		if (a_Token.value == "fonttbl")
		{
			m_State->group_container = a_State.group_current;
			m_State->group_parent = a_State.group_current ? a_State.group_current->parent : nullptr;
			m_State->table_started = true;

			return eResult_Handled;
		}
		else if (a_Token.value == "deff")
		{
			if (a_Token.parameter < 0)
			{
				return eResult_Invalid;
			}

			SetDefault(a_Token.parameter);

			return eResult_Handled;
		}
		else if (a_Token.value == "f")
		{
			if (!m_State->table_started || a_Token.parameter < 0)
			{
				return eResult_Invalid;
			}

			RtfFont* font = GetFont(a_Token.parameter);
			a_State.target_current = font;

			return eResult_Handled;
		}
		else
		{
			return eResult_Propagate;
		}
	}

}; // namespace ExLibris