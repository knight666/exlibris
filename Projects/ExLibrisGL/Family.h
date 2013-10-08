#ifndef _FAMILY_H_
#define _FAMILY_H_

#include <set>

#include "IFont.h"

namespace ExLibris
{
	class Library;
};

namespace ExLibris
{

	class Family
	{
	
	public:
	
		Family(Library* a_Library, const std::string& a_Name);
		~Family();
	
		Library* GetLibrary() const;

		const std::string& GetName() const;

		size_t GetFontCount() const;

		void AddFont(IFont* a_Font);
		IFont* FindFont(Weight a_Weight, Style a_Style) const;

	private:

		Library* m_Library;
		std::string m_Name;
		std::set<IFont*> m_Fonts;
	
	}; // class Family

}; // namespace ExLibris

#endif