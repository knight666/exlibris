#include "ExLibrisGL.PCH.h"

#include "FontFreetype.h"

#include "FontFaceFreetype.h"

namespace ExLibris
{

	FontFreetype::FontFreetype(const std::string& a_Family)
		: m_Font(nullptr)
		, m_FamilyName(a_Family)
	{
	}
	
	FontFreetype::~FontFreetype()
	{
		if (m_Font != nullptr)
		{
			FT_Done_Face(m_Font);
		}
	}

	const std::string& FontFreetype::GetFamilyName() const
	{
		return m_FamilyName;
	}

	bool FontFreetype::LoadFontData(FT_Face a_Font)
	{
		if (a_Font == nullptr)
		{
			return false;
		}

		FT_Error errors = 0;
		
		errors = FT_Select_Charmap(a_Font, FT_ENCODING_UNICODE);
		if (errors != 0 || a_Font->charmap == nullptr)
		{
			return false;
		}

		m_Font = a_Font;

		return true;
	}

	FontFaceFreetype* FontFreetype::CreateFace(float a_Size) const
	{
		if (m_Font == nullptr)
		{
			return nullptr;
		}

		FontFaceFreetype* face = new FontFaceFreetype(m_Font);
		if (!face->LoadGlyphs(a_Size))
		{
			delete face;
			return nullptr;
		}

		return face;
	}

}; // namespace ExLibris