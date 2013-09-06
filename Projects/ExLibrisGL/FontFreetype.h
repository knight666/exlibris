#ifndef _FONTFREETYPE_H_
#define _FONTFREETYPE_H_

#include <ft2build.h>
#include <freetype/freetype.h>

#include "IFont.h"

namespace ExLibris
{

	class FontFreetype
		: public IFont
	{
	
	public:
	
		FontFreetype(const std::string& a_FamilyName);
		~FontFreetype();

		unsigned int GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const;

		FontFace* CreateFace(float a_Size) const;

		bool LoadFontData(FT_Face a_Font);

	private:

		FT_Face m_Font;
	
	}; // class FontFreetype

}; // namespace ExLibris

#endif