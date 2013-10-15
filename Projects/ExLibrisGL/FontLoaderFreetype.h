#ifndef _FONTLOADERFREETYPE_H_
#define _FONTLOADERFREETYPE_H_

#include <vector>

#include "FontFreetype.h"
#include "IFontLoader.h"

namespace ExLibris
{

	class FontLoaderFreetype
		: public IFontLoader
	{
	
	public:
	
		FontLoaderFreetype(Library* a_Library);
		~FontLoaderFreetype();

		FT_Library GetLibrary() const;

		IFont* LoadFont(const std::string& a_Path);
	
	private:

		FT_Error m_Error;
		FT_Library m_FTLibrary;
	
	}; // class FontLoaderFreetype

}; // namespace ExLibris

#endif