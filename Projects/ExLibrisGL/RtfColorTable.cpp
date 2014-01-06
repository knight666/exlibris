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

	RtfColorTable::RtfColorTable()
	{
	}

	RtfColorTable::~RtfColorTable()
	{
	}

	size_t RtfColorTable::GetColorCount() const
	{
		return m_Colors.size();
	}

	void RtfColorTable::AddColor(const RtfColor& a_Color)
	{
		m_Colors.push_back(a_Color);
	}

	RtfColor RtfColorTable::GetColor(int a_Index, const RtfColor& a_Default /*= RtfColor(0, 0, 0)*/) const
	{
		if (a_Index < 0 || a_Index >= (int)m_Colors.size())
		{
			return a_Default;
		}
		
		return m_Colors[a_Index];
	}

	bool RtfColorTable::TryGetColor(RtfColor& a_Target, int a_Index) const
	{
		if (a_Index < 0 || a_Index >= (int)m_Colors.size())
		{
			return false;
		}

		a_Target = m_Colors[a_Index];

		return true;
	}

}; // namespace ExLibris