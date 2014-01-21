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

#include "RtfColorTable.h"

#include "RtfDomDocument.h"

namespace ExLibris
{

	struct RtfColorTable::ParseState
	{
		ParseState()
			: group_container(nullptr)
			, group_parent(nullptr)
			, color(nullptr)
			, color_index(0)
			, table_started(false)
		{
		}

		RtfParserGroup* group_container;
		RtfParserGroup* group_parent;
		RtfColor* color;
		int color_index;
		bool table_started;
	};

	RtfColorTable::RtfColorTable(RtfDomDocument& a_Document)
		: IRtfParseable(&m_Document)
		, m_Document(a_Document)
		, m_IndexNext(1)
		, m_IndexDefault(0)
		, m_ColorDefault(new RtfColor(0, 0, 0))
		, m_State(new ParseState)
	{
		m_Colors.insert(std::make_pair(0, m_ColorDefault));
	}

	RtfColorTable::~RtfColorTable()
	{
		for (std::map<int, RtfColor*>::iterator color_it = m_Colors.begin(); color_it != m_Colors.end(); ++color_it)
		{
			delete color_it->second;
		}
		m_Colors.clear();

		delete m_State;
	}

	size_t RtfColorTable::GetColorCount() const
	{
		return m_Colors.size();
	}

	RtfColor* RtfColorTable::GetColor(int a_Index)
	{
		RtfColor* color = nullptr;

		if (a_Index < 0)
		{
			return nullptr;
		}
		else if (a_Index >= m_IndexNext)
		{
			color = new RtfColor(*GetDefaultColor());
			m_Colors.insert(std::make_pair(a_Index, color));

			m_IndexNext = a_Index + 1;
		}
		else
		{
			std::map<int, RtfColor*>::iterator found = m_Colors.find(a_Index);
			if (found != m_Colors.end())
			{
				color = found->second;
			}
			else
			{
				color = new RtfColor(*GetDefaultColor());
				m_Colors.insert(std::make_pair(a_Index, color));
			}
		}

		return color;
	}

	RtfColor* RtfColorTable::AddColor(const RtfColor& a_Color)
	{
		RtfColor* color = new RtfColor(a_Color);

		m_Colors.insert(std::make_pair(m_IndexNext++, color));

		return color;
	}

	RtfColor* RtfColorTable::GetDefaultColor() const
	{
		return m_ColorDefault;
	}

	void RtfColorTable::SetDefaultColor(int a_Index)
	{
		if (a_Index < 0)
		{
			return;
		}

		m_ColorDefault = GetColor(a_Index);
	}

	IRtfParseable::Result RtfColorTable::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		if (a_Token.value == "colortbl")
		{
			m_State->group_container = a_State.group_current;
			m_State->group_parent = a_State.group_current ? a_State.group_current->parent : nullptr;
			m_State->color_index = 0;
			m_State->color = GetColor(0);
			m_State->table_started = true;

			return eResult_Handled;
		}
		else if (a_Token.value == "red")
		{
			if (!m_State->table_started || a_Token.parameter < 0 || a_Token.parameter > 255)
			{
				return eResult_Invalid;
			}

			m_State->color->r = a_Token.parameter;

			return eResult_Handled;
		}
		else if (a_Token.value == "green")
		{
			if (!m_State->table_started || a_Token.parameter < 0 || a_Token.parameter > 255)
			{
				return eResult_Invalid;
			}

			m_State->color->g = a_Token.parameter;

			return eResult_Handled;
		}
		else if (a_Token.value == "blue")
		{
			if (!m_State->table_started || a_Token.parameter < 0 || a_Token.parameter > 255)
			{
				return eResult_Invalid;
			}

			m_State->color->b = a_Token.parameter;

			return eResult_Handled;
		}
		else
		{
			return (m_State->table_started) ? eResult_Invalid : eResult_Propagate;
		}
	}

	IRtfParseable::Result RtfColorTable::_ParseValue(RtfParserState& a_State, const RtfToken& a_Token)
	{
		if (m_State->table_started)
		{
			m_State->color = GetColor(++m_State->color_index);

			return eResult_Handled;
		}
		else
		{
			return eResult_Invalid;
		}
	}

}; // namespace ExLibris