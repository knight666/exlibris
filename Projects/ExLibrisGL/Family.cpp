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

#include "Family.h"

namespace ExLibris
{

	Family::Family(Library* a_Library, const std::string& a_Name)
		: m_Library(a_Library)
		, m_Name(a_Name)
	{
	}
	
	Family::~Family()
	{
		for (std::set<IFont*>::iterator font_it = m_Fonts.begin(); font_it != m_Fonts.end(); ++font_it)
		{
			delete *font_it;
		}
		m_Fonts.clear();
	}

	Library* Family::GetLibrary() const
	{
		return m_Library;
	}

	const std::string& Family::GetName() const
	{
		return m_Name;
	}

	size_t Family::GetFontCount() const
	{
		return m_Fonts.size();
	}

	void Family::AddFont(IFont* a_Font)
	{
		std::set<IFont*>::iterator found = m_Fonts.find(a_Font);
		if (found == m_Fonts.end())
		{
			m_Fonts.insert(a_Font);
		}
	}

	IFont* Family::FindFont(Weight a_Weight, Style a_Style) const
	{
		IFont* match = nullptr;

		for (std::set<IFont*>::const_iterator font_it = m_Fonts.begin(); font_it != m_Fonts.end(); ++font_it)
		{
			IFont* font = *font_it;

			int score = 0;

			if (font->GetStyle() == a_Style)
			{
				match = font;
				score++;
			}

			if (font->GetWeight() == a_Weight)
			{
				match = font;
				score++;
			}

			if (score == 2)
			{
				break;
			}
		}

		return match;
	}

}; // namespace ExLibris