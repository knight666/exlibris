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

namespace ExLibris
{

	RtfColorTable::RtfColorTable(RtfDomDocument& a_Document)
		: m_Document(a_Document)
		, m_IndexNext(1)
		, m_IndexDefault(0)
		, m_ColorDefault(new RtfColor(0, 0, 0))
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

}; // namespace ExLibris