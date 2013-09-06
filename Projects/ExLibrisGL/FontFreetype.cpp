#include "ExLibrisGL.PCH.h"

#include "FontFreetype.h"

#include "FontFace.h"
#include "FreetypeConversion.h"
#include "Glyph.h"

namespace ExLibris
{

	FontFreetype::FontFreetype(const std::string& a_Family)
		: IFont(a_Family)
		, m_Font(nullptr)
	{
	}
	
	FontFreetype::~FontFreetype()
	{
		if (m_Font != nullptr)
		{
			FT_Done_Face(m_Font);
		}
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

	FontFace* FontFreetype::CreateFace(float a_Size) const
	{
		if (m_Font == nullptr)
		{
			return nullptr;
		}

		FT_Error errors = 0;

		errors = FT_Set_Char_Size(m_Font, 0, Fixed26Dot6::ToFixed(a_Size), 0, 96);
		if (errors != 0)
		{
			return nullptr;
		}

		FontFace* face = new FontFace(this);
		face->SetSize(a_Size);
		face->SetLineHeight(Fixed26Dot6::ToFloat(m_Font->size->metrics.height));

		FT_UInt codepoint = 0;
		FT_ULong glyph_index = FT_Get_First_Char(m_Font, &codepoint);
		do
		{
			errors = FT_Load_Glyph(m_Font, codepoint, FT_LOAD_DEFAULT);
			if (errors == 0)
			{
				Glyph* glyph = new Glyph;
				glyph->index = (unsigned int)codepoint;

				_LoadMetrics(m_Font->glyph, glyph);
				_LoadBitmap(m_Font->glyph, glyph);

				face->AddGlyph(glyph);
			}

			glyph_index = FT_Get_Next_Char(m_Font, glyph_index, &codepoint);
		}
		while (codepoint != 0);

		return face;
	}

	bool FontFreetype::_LoadMetrics(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const
	{
		FT_Error errors = 0;

		FT_Glyph_Metrics& glyph_metrics = a_Slot->metrics;

		a_Glyph->metrics = new GlyphMetrics;
		a_Glyph->metrics->offset.x = Fixed26Dot6::ToFloat(glyph_metrics.horiBearingX);
		a_Glyph->metrics->offset.y = Fixed26Dot6::ToFloat(glyph_metrics.vertAdvance - glyph_metrics.horiBearingY);
		a_Glyph->metrics->advance = Fixed26Dot6::ToFloat(glyph_metrics.horiAdvance);

		// bounding box

		FT_BBox bounding_box;
		errors = FT_Outline_Get_BBox(&a_Slot->outline, &bounding_box);
		if (errors == 0)
		{
			a_Glyph->metrics->bounding_box.minimum.x = Fixed26Dot6::ToFloat(bounding_box.xMin);
			a_Glyph->metrics->bounding_box.minimum.y = Fixed26Dot6::ToFloat(bounding_box.yMin);
			a_Glyph->metrics->bounding_box.maximum.x = Fixed26Dot6::ToFloat(bounding_box.xMax);
			a_Glyph->metrics->bounding_box.maximum.y = Fixed26Dot6::ToFloat(bounding_box.yMax);
		}

		return true;
	}

	bool FontFreetype::_LoadBitmap(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const
	{
		FT_Error errors = 0;

		errors = FT_Render_Glyph(a_Slot, FT_RENDER_MODE_NORMAL);
		if (errors != 0)
		{
			return false;
		}

		FT_Bitmap& glyph_bitmap = a_Slot->bitmap;

		unsigned int data_length = glyph_bitmap.width * glyph_bitmap.rows * 4;
		if (data_length == 0)
		{
			return false;
		}

		a_Glyph->bitmap = new GlyphBitmap;
		a_Glyph->bitmap->width = glyph_bitmap.width;
		a_Glyph->bitmap->height = glyph_bitmap.rows;
		a_Glyph->bitmap->data = new unsigned char[data_length];

		unsigned char* src_line = glyph_bitmap.buffer;
		unsigned int src_pitch = glyph_bitmap.pitch;

		unsigned char* dst_line = a_Glyph->bitmap->data;
		unsigned int dst_pitch = a_Glyph->bitmap->width * 4;

		for (int y = 0; y < glyph_bitmap.rows; ++y)
		{
			unsigned char* src = src_line;
			unsigned char* dst = dst_line;

			for (int x = 0; x < glyph_bitmap.width; ++x)
			{
				char value = *src;
				dst[0] = value;
				dst[1] = value;
				dst[2] = value;
				dst[3] = value;

				dst += 4;
				src++;
			}

			src_line += src_pitch;
			dst_line += dst_pitch;
		}

		return true;
	}

	bool FontFreetype::_LoadOutline(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const
	{
		return false;
	}

}; // namespace ExLibris