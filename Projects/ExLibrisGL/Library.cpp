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