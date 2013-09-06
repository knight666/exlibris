#ifndef _FONTFREETYPE_H_
#define _FONTFREETYPE_H_

#include <ft2build.h>
#include <freetype/freetype.h>

namespace ExLibris
{
	class FontFace;
}

namespace ExLibris
{

	class FontFreetype
	{
	
	public:
	
		FontFreetype(const std::string& a_FamilyName);
		~FontFreetype();

		const std::string& GetFamilyName() const;

		unsigned int GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const;

		bool LoadFontData(FT_Face a_Font);
	
		FontFace* CreateFace(float a_Size) const;

	private:

		FT_Face m_Font;
		std::string m_FamilyName;
	
	}; // class FontFreetype

}; // namespace ExLibris

#endif