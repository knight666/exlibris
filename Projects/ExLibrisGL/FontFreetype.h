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
	
		FontFreetype(Family* a_Family, FT_Byte* a_Buffer, size_t a_BufferSize);
		~FontFreetype();

		unsigned int GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const;

		bool LoadFontData(FT_Face a_Font);

		FontFace* CreateFace(float a_Size);

	private:

		bool _LoadMetrics(FT_GlyphSlot a_Slot, Glyph* a_Glyph, FT_Size_Metrics a_FontMetrics) const;
		bool _LoadBitmap(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const;
		bool _LoadOutline(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const;

	private:

		FT_Face m_Font;
		FT_Byte* m_Buffer;
		size_t m_BufferSize;

		FT_Outline_Funcs m_OutlineCallbacks;

		std::map<float, FontFace*> m_Faces;
	
	}; // class FontFreetype

}; // namespace ExLibris

#endif