#ifndef _FONTLOADERFREETYPE_H_
#define _FONTLOADERFREETYPE_H_

#include "FontFreetype.h"

namespace ExLibris
{
	class Library;
};

namespace ExLibris
{

	class FontLoaderFreetype
	{
	
	public:
	
		FontLoaderFreetype(Library* a_Library);
		~FontLoaderFreetype();

		FT_Library GetLibrary() const;

		FontFreetype* LoadFont(const std::string& a_Path);
	
	private:

		Library* m_Library;
		FT_Error m_Error;
		FT_Library m_FTLibrary;
	
	}; // class FontLoaderFreetype

}; // namespace ExLibris

#endif