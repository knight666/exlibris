#include "ExLibrisGL.PCH.h"

#include "Library.h"

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