#include "ExLibrisGL.PCH.h"

#include "FontFreetype.h"

#include "FontFaceFreetype.h"
#include "Glyph.h"

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

	unsigned int FontFreetype::GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const
	{
		if (m_Font == nullptr)
		{
			return 0;
		}

		return (unsigned int)FT_Get_Char_Index(m_Font, (FT_ULong)a_CodepointUtf32);
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

		FT_Error errors = 0;

		errors = FT_Set_Char_Size(m_Font, 0, ((FT_F26Dot6)a_Size) << 6, 0, 96);
		if (errors != 0)
		{
			return nullptr;
		}

		FontFaceFreetype* face = new FontFaceFreetype(this);
		face->SetSize(a_Size);
		face->SetLineHeight((float)(m_Font->size->metrics.height >> 6));

		FT_UInt codepoint = 0;
		FT_ULong glyph_index = FT_Get_First_Char(m_Font, &codepoint);
		do
		{
			errors = FT_Load_Glyph(m_Font, codepoint, FT_LOAD_DEFAULT);
			if (errors != 0)
			{
				delete face;
				return nullptr;
			}

			FT_Glyph_Metrics& glyph_metrics = m_Font->glyph->metrics;

			Glyph* glyph = new Glyph;
			glyph->index = (unsigned int)codepoint;

			glyph->metrics = new GlyphMetrics;
			glyph->metrics->offset.x = (float)((glyph_metrics.horiBearingX) >> 6);
			glyph->metrics->offset.y = (float)((glyph_metrics.vertAdvance - glyph_metrics.horiBearingY) >> 6);
			glyph->metrics->advance = (float)((glyph_metrics.horiAdvance) >> 6);

			face->AddGlyph(glyph);

			glyph_index = FT_Get_Next_Char(m_Font, glyph_index, &codepoint);
		}
		while (codepoint != 0);

		return face;
	}

}; // namespace ExLibris