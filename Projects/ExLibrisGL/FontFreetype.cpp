#include "ExLibrisGL.PCH.h"

#include "FontFreetype.h"

#include "FontFaceFreetype.h"

namespace ExLibris
{

	FontFreetype::FontFreetype(FT_Face a_Font)
		: m_Font(a_Font)
	{
	}
	
	FontFreetype::~FontFreetype()
	{
	}

	FontFaceFreetype* FontFreetype::CreateFace(float a_Size) const
	{
		FontFaceFreetype* face = new FontFaceFreetype(m_Font);
		if (!face->LoadGlyphs(a_Size))
		{
			delete face;
			return nullptr;
		}

		return face;
	}

}; // namespace ExLibris