#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include "Family.h"

namespace ExLibris
{

	class Library
	{
	
	public:
	
		Library();
		~Library();

		size_t GetFamilyCount() const;
	
		Family* CreateFamily(const std::string& a_Name);
		Family* FindFamily(const std::string& a_Name) const;

	private:

		std::map<std::string, Family*> m_Families;
	
	}; // class Library

}; // namespace ExLibris

#endif