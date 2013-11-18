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

#include "Exception.h"
#include "FaceOptions.h"
#include "Family.h"
#include "GlyphProviderSystem.h"
#include "IFontLoader.h"
#include "IGlyphProvider.h"

namespace ExLibris
{

	Library::Library()
	{
		Family* family = CreateFamily("System");
		family->AddGlyphProvider(new GlyphProviderSystem(this));
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

	bool Library::MapFontToFile(const std::string& a_Path) const
	{
		std::fstream file_stream(a_Path, std::ios::in | std::ios::binary);
		if (!file_stream.is_open())
		{
			return false;
		}

		bool result = MapFontToStream(file_stream);

		file_stream.close();

		return result;
	}

	bool Library::MapFontToStream(std::istream& a_Stream) const
	{
		for (std::vector<IFontLoader*>::const_iterator loader_it = m_Loaders.begin(); loader_it != m_Loaders.end(); ++loader_it)
		{
			IFontLoader* loader = *loader_it;

			IGlyphProvider* provider = provider = loader->LoadGlyphProvider(a_Stream);
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

	FontFace* Library::RequestFontFace(const FaceRequest& a_Request)
	{
		Family* family = nullptr;
		if (a_Request.HasFamilyName())
		{
			family = FindFamily(a_Request.GetFamilyName());
		}
		else
		{
			family = FindFamily("System");
		}

		if (family == nullptr)
		{
			std::stringstream ss;
			ss << "Could not find family named \"" << a_Request.GetFamilyName() << "\".";
			EXL_THROW("Library::RequestFontFace", ss.str().c_str());

			return nullptr;
		}

		Weight weight = eWeight_Normal;
		if (a_Request.HasWeight())
		{
			weight = a_Request.GetWeight();
		}

		Style style = eStyle_None;
		if (a_Request.HasStyle())
		{
			style = a_Request.GetStyle();
		}

		IFont* font = nullptr; //family->FindFont(weight, style);
		if (font == nullptr)
		{
			std::stringstream ss;
			ss << "Could not find font with specified weight and style.";
			EXL_THROW("Library::RequestFontFace", ss.str().c_str());

			return nullptr;
		}

		FaceOptions options;

		if (a_Request.HasSize())
		{
			options.size = a_Request.GetSize();
		}
		else
		{
			options.size = 10.0f;
		}

		return nullptr;
	}

	Face* Library::RequestFace(const FaceRequest& a_Request) const
	{
		Family* family = nullptr;
		if (a_Request.HasFamilyName())
		{
			family = FindFamily(a_Request.GetFamilyName());

			if (family == nullptr)
			{
				std::stringstream ss;
				ss << "Could not find family named \"" << a_Request.GetFamilyName() << "\".";
				EXL_THROW("Library::RequestFace", ss.str().c_str());

				return nullptr;
			}
		}
		else
		{
			family = FindFamily("System");
		}

		Weight weight = eWeight_Normal;
		if (a_Request.HasWeight())
		{
			weight = a_Request.GetWeight();
		}

		Style style = eStyle_None;
		if (a_Request.HasStyle())
		{
			style = a_Request.GetStyle();
		}

		float size = 10.0f;
		if (a_Request.HasSize())
		{
			size = a_Request.GetSize();
		}

		IGlyphProvider* provider = family->FindGlyphProvider(size, weight, style);
		if (provider == nullptr)
		{
			return nullptr;
		}

		return provider->CreateFace(size);
	}

}; // namespace ExLibris