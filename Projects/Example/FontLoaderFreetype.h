#ifndef _FONTLOADERFREETYPE_H_
#define _FONTLOADERFREETYPE_H_

#include <ft2build.h>
#include <freetype/freetype.h>

#include <string>

namespace ExLibris
{

	class FontLoaderFreetype
	{
	
	public:
	
		FontLoaderFreetype();
		~FontLoaderFreetype();

		FT_Face LoadFontFace(const std::string& a_Path);
	
	private:

		FT_Error m_Error;
		FT_Library m_Library;
	
	}; // class FontLoaderFreetype

}; // namespace ExLibris

#endif