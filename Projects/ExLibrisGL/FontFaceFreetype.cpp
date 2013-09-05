#include "ExLibrisGL.PCH.h"

#include "FontFaceFreetype.h"

#include "FontFreetype.h"

namespace ExLibris
{

	FontFaceFreetype::FontFaceFreetype(FontFreetype* a_Font)
		: m_Font(a_Font)
		, m_Size(0.0f)
	{
	}
	
	FontFaceFreetype::~FontFaceFreetype()
	{
	}

	FontFreetype* FontFaceFreetype::GetFont() const
	{
		return m_Font;
	}

	float FontFaceFreetype::GetSize() const
	{
		return m_Size;
	}

	bool FontFaceFreetype::LoadGlyphs(FT_Face a_FontData, float a_Size)
	{
		return false;
	}

}; // namespace ExLibris