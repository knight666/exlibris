#include "ExLibrisGL.PCH.h"

#include "FontFaceFreetype.h"

#include "FontFreetype.h"
#include "Glyph.h"

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

	float FontFaceFreetype::GetLineHeight() const
	{
		return m_LineHeight;
	}

	Glyph* FontFaceFreetype::FindGlyph(unsigned int a_CodepointUtf32) const
	{
		unsigned int codepoint = (unsigned int)m_Font->GetIndexFromCodepoint(a_CodepointUtf32);
		if (codepoint == 0)
		{
			return nullptr;
		}

		std::map<unsigned int, Glyph*>::const_iterator found = m_Glyphs.find(codepoint);
		if (found != m_Glyphs.end())
		{
			return found->second;
		}

		return nullptr;
	}

	bool FontFaceFreetype::LoadGlyphs(FT_Face a_FontData, float a_Size)
	{
		FT_Error errors = 0;

		m_Size = a_Size;

		errors = FT_Set_Char_Size(a_FontData, 0, ((FT_F26Dot6)a_Size) << 6, 0, 96);
		m_LineHeight = (float)(a_FontData->size->metrics.height >> 6);

		FT_UInt codepoint = 0;
		FT_ULong glyph_index = FT_Get_First_Char(a_FontData, &codepoint);
		do
		{
			errors = FT_Load_Glyph(a_FontData, codepoint, FT_LOAD_DEFAULT);
			FT_Glyph_Metrics& glyph_metrics = a_FontData->glyph->metrics;

			Glyph* glyph = new Glyph;
			glyph->index = (unsigned int)codepoint;

			glyph->metrics = new GlyphMetrics;
			glyph->metrics->offset.x = (float)((glyph_metrics.horiBearingX) >> 6);
			glyph->metrics->offset.y = (float)((glyph_metrics.vertAdvance - glyph_metrics.horiBearingY) >> 6);
			glyph->metrics->advance = (float)((glyph_metrics.horiAdvance) >> 6);

			m_Glyphs.insert(std::make_pair(glyph->index, glyph));

			glyph_index = FT_Get_Next_Char(a_FontData, glyph_index, &codepoint);
		}
		while (codepoint != 0);

		return true;
	}

}; // namespace ExLibris