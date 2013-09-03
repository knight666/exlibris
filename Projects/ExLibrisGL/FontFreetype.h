#ifndef _FONTFREETYPE_H_
#define _FONTFREETYPE_H_

#include <ft2build.h>
#include <freetype/freetype.h>

namespace ExLibris
{
	class FontFaceFreetype;
}

namespace ExLibris
{

	class FontFreetype
	{
	
	public:
	
		FontFreetype(FT_Face a_Font);
		~FontFreetype();
	
		FontFaceFreetype* CreateFace(float a_Size) const;

	private:

		FT_Face m_Font;
	
	}; // class FontFreetype

}; // namespace ExLibris

#endif