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
	
		FontFreetype(Family* a_Family);
		~FontFreetype();

		unsigned int GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const;

		bool LoadFontData(FT_Face a_Font);

	private:

		FontFace* _CreateFace(float a_Size);

	private:

		bool _LoadMetrics(FT_GlyphSlot a_Slot, Glyph* a_Glyph, FT_Size_Metrics a_FontMetrics) const;
		bool _LoadBitmap(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const;
		bool _LoadOutline(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const;

	private:

		FT_Face m_Font;
		FT_Outline_Funcs m_OutlineCallbacks;
	
	}; // class FontFreetype

}; // namespace ExLibris

#endif