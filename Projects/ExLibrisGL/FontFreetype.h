#ifndef _FONTFREETYPE_H_
#define _FONTFREETYPE_H_

#include <ft2build.h>
#include <freetype/freetype.h>

#include "IFont.h"

namespace ExLibris
{
	struct Glyph;
}

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

		bool _LoadMetrics(FT_GlyphSlot a_Slot, Glyph* a_Glyph, FT_Pos a_Ascender) const;
		bool _LoadBitmap(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const;
		bool _LoadOutline(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const;

	private:

		FT_Face m_Font;
		FT_Outline_Funcs m_OutlineCallbacks;
	
	}; // class FontFreetype

}; // namespace ExLibris

#endif