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

#include "Memory/Memory.h"
#include "Exception.h"
#include "FaceOptions.h"
#include "Family.h"
#include "GlyphProviderSystem.h"
#include "IFontLoader.h"
#include "IGlyphProvider.h"

namespace ExLibris {

	Library::Library()
	{
		Family* family = CreateFamily("System");
		family->AddGlyphProvider(new GlyphProviderSystem(this));
	}
	
	Library::~Library()
	{
		for (std::map<String, Family*>::iterator family_it = m_Families.begin(); family_it != m_Families.end(); ++family_it)
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

	void Library::AddLoader(IFontLoader* loader)
	{
		if (std::find(m_Loaders.begin(), m_Loaders.end(), loader) == m_Loaders.end())
		{
			m_Loaders.push_back(loader);
		}
	}

	bool Library::MapFontToFile(const String& path) const
	{
		std::fstream file_stream(path.c_str(), std::ios::in | std::ios::binary);
		if (!file_stream.is_open())
		{
			return false;
		}

		bool result = MapFontToStream(file_stream);

		file_stream.close();

		return result;
	}

	bool Library::MapFontToStream(std::istream& stream) const
	{
		for (IFontLoader* loader : m_Loaders)
		{
			IGlyphProvider* provider = provider = loader->LoadGlyphProvider(stream);
			if (provider != nullptr)
			{
				if (provider->GetFamily() != nullptr)
				{
					provider->GetFamily()->AddGlyphProvider(provider);
				}

				return true;
			}
		}

		return false;
	}

	size_t Library::GetFamilyCount() const
	{
		return m_Families.size();
	}

	Family* Library::CreateFamily(const String& name)
	{
		FamilyMap::iterator found = m_Families.find(name);
		if (found != m_Families.end())
		{
			return found->second;
		}
		else
		{
			Family* family = new Family(this, name);
			m_Families.insert(std::make_pair(name, family));

			return family;
		}
	}

	Family* Library::FindFamily(const String& name) const
	{
		FamilyMap::const_iterator found = m_Families.find(name);
		if (found != m_Families.end())
		{
			return found->second;
		}
		else
		{
			return nullptr;
		}
	}

	Face* Library::RequestFace(const FaceRequest& request) const
	{
		Family* family = nullptr;
		if (request.HasFamilyName())
		{
			family = FindFamily(request.GetFamilyName());

			if (family == nullptr)
			{
				StringStream ss;
				ss << "Could not find family named \"" << request.GetFamilyName() << "\".";
				EXL_THROW("Library::RequestFace", ss.str().c_str());

				return nullptr;
			}
		}
		else
		{
			family = FindFamily("System");
		}

		Weight weight = eWeight_Normal;
		if (request.HasWeight())
		{
			weight = request.GetWeight();
		}

		Style style = eStyle_None;
		if (request.HasStyle())
		{
			style = request.GetStyle();
		}

		float size = 10.0f;
		if (request.HasSize())
		{
			size = request.GetSize();
		}

		IGlyphProvider* provider = family->FindGlyphProvider(size, weight, style);
		if (provider == nullptr)
		{
			return nullptr;
		}

		return provider->CreateFace(size);
	}

};