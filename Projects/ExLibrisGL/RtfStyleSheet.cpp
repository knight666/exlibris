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

	RtfStyleSheet::RtfStyleSheet()
	{
	}

	RtfStyleSheet::~RtfStyleSheet()
	{
		for (std::map<int, RtfStyle*>::iterator style_it = m_Styles.begin(); style_it != m_Styles.end(); ++style_it)
		{
			delete style_it->second;
		}
		m_Styles.clear();
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

		RtfStyle* style = new RtfStyle();
		
		m_Styles.insert(std::make_pair(a_Index, style));

		return style;
	}

}; // namespace ExLibris