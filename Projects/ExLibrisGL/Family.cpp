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

#include "IGlyphProvider.h"

namespace ExLibris
{

	Family::Family(Library* a_Library, const std::string& a_Name)
		: m_Library(a_Library)
		, m_Name(a_Name)
	{
	}
	
	Family::~Family()
	{
		for (std::set<IGlyphProvider*>::iterator provider_it = m_GlyphProviders.begin(); provider_it != m_GlyphProviders.end(); ++provider_it)
		{
			delete *provider_it;
		}
		m_GlyphProviders.clear();
	}

	Library* Family::GetLibrary() const
	{
		return m_Library;
	}

	const std::string& Family::GetName() const
	{
		return m_Name;
	}

	size_t Family::GetGlyphProviderCount() const
	{
		return m_GlyphProviders.size();
	}

	void Family::AddGlyphProvider(IGlyphProvider* a_Provider)
	{
		if (a_Provider == nullptr)
		{
			return;
		}

		std::set<IGlyphProvider*>::iterator found = m_GlyphProviders.find(a_Provider);
		if (found == m_GlyphProviders.end())
		{
			m_GlyphProviders.insert(a_Provider);
		}
	}

	IGlyphProvider* Family::FindGlyphProvider(float a_Size, Weight a_Weight, Style a_Style) const
	{
		IGlyphProvider* match = nullptr;
		int score_highest = -1;

		for (std::set<IGlyphProvider*>::const_iterator provider_it = m_GlyphProviders.begin(); provider_it != m_GlyphProviders.end(); ++provider_it)
		{
			IGlyphProvider* provider = *provider_it;

			int score = provider->GetMatchScore(a_Size, a_Weight, a_Style);
			if (score >= score_highest)
			{
				match = provider;
				score_highest = score;
			}
		}

		return match;
	}

}; // namespace ExLibris