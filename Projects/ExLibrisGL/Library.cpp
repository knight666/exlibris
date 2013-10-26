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

#include "Library.h"

#include "Family.h"
#include "IFontLoader.h"

namespace ExLibris
{

	Library::Library()
	{
	}
	
	Library::~Library()
	{
		for (std::map<std::string, Family*>::iterator family_it = m_Families.begin(); family_it != m_Families.end(); ++family_it)
		{
			delete family_it->second;
		}
		m_Families.clear();

		for (std::vector<IFontLoader*>::iterator loader_it = m_Loaders.begin(); loader_it != m_Loaders.end(); ++loader_it)
		{
			delete *loader_it;
		}
		m_Loaders.clear();
	}

	size_t Library::GetLoaderCount() const
	{
		return m_Loaders.size();
	}

	void Library::AddLoader(IFontLoader* a_Loader)
	{
		for (std::vector<IFontLoader*>::iterator loader_it = m_Loaders.begin(); loader_it != m_Loaders.end(); ++loader_it)
		{
			IFontLoader* loader = *loader_it;

			if (loader == a_Loader)
			{
				return;
			}
		}

		m_Loaders.push_back(a_Loader);
	}

	IFont* Library::LoadFont(const std::string& a_Path)
	{
		IFont* font_loaded = nullptr;

		for (std::vector<IFontLoader*>::iterator loader_it = m_Loaders.begin(); loader_it != m_Loaders.end(); ++loader_it)
		{
			IFontLoader* loader = *loader_it;

			font_loaded = loader->LoadFont(a_Path);
			if (font_loaded != nullptr)
			{
				break;
			}
		}

		return font_loaded;
	}

	size_t Library::GetFamilyCount() const
	{
		return m_Families.size();
	}

	Family* Library::CreateFamily(const std::string& a_Name)
	{
		std::map<std::string, Family*>::iterator found = m_Families.find(a_Name);
		if (found != m_Families.end())
		{
			return found->second;
		}
		else
		{
			Family* family = new Family(this, a_Name);
			m_Families.insert(std::make_pair(a_Name, family));

			return family;
		}
	}

	Family* Library::FindFamily(const std::string& a_Name) const
	{
		std::map<std::string, Family*>::const_iterator found = m_Families.find(a_Name);
		if (found != m_Families.end())
		{
			return found->second;
		}
		else
		{
			return nullptr;
		}
	}

}; // namespace ExLibris