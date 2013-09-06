#include "ExLibrisGL.PCH.h"

#include "FontFaceFreetype.h"

#include "FontFreetype.h"

namespace ExLibris
{

	FontFaceFreetype::FontFaceFreetype(const FontFreetype* a_Font)
		: m_Font(a_Font)
		, m_Size(0.0f)
		, m_LineHeight(0.0f)
	{
	}
	
	FontFaceFreetype::~FontFaceFreetype()
	{
	}

	const FontFreetype* FontFaceFreetype::GetFont() const
	{
		return m_Font;
	}

	float FontFaceFreetype::GetSize() const
	{
		return m_Size;
	}

	bool FontFaceFreetype::LoadGlyphs(FT_Face a_FontData, float a_Size)
	{
		FT_Error errors = 0;

		errors = FT_Set_Char_Size(a_FontData, 0, ((FT_F26Dot6)a_Size) << 6, 0, 96);
		m_LineHeight = (float)(a_FontData->size->metrics.height >> 6);

		return true;
	}

	float FontFaceFreetype::GetLineHeight() const
	{
		return m_LineHeight;
	}

}; // namespace ExLibris