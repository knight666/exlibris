#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <vector>

namespace ExLibris
{
	class Family;
	class IFont;
	class IFontLoader;
}

namespace ExLibris
{

	class Library
	{
	
	public:
	
		Library();
		~Library();

		size_t GetLoaderCount() const;
		void AddLoader(IFontLoader* a_Loader);

		IFont* LoadFont(const std::string& a_Path);

		size_t GetFamilyCount() const;
		Family* CreateFamily(const std::string& a_Name);
		Family* FindFamily(const std::string& a_Name) const;

	private:

		std::vector<IFontLoader*> m_Loaders;
		std::map<std::string, Family*> m_Families;
	
	}; // class Library

}; // namespace ExLibris

#endif