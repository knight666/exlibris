#include "ExLibrisGL.PCH.h"

#include "FontFreetype.h"

#include "FontFace.h"
#include "FreetypeConversion.h"
#include "Glyph.h"

namespace ExLibris
{

	int CallbackMoveTo(const FT_Vector* a_To, void* a_User)
	{
		GlyphOutline* outline = (GlyphOutline*)a_User;

		GlyphContour* contour = new GlyphContour;
		outline->contours.push_back(contour);

		glm::vec2 position = Fixed26Dot6::ToFloatVec2(a_To);
		contour->points.push_back(position);

		return 0;
	}

	int CallbackLineTo(const FT_Vector* a_To, void* a_User)
	{
		GlyphOutline* outline = (GlyphOutline*)a_User;
		if (outline == nullptr || outline->contours.size() == 0)
		{
			return -1;
		}

		GlyphContour* contour = outline->contours.back();

		glm::vec2 position = Fixed26Dot6::ToFloatVec2(a_To);
		contour->points.push_back(position);

		return 0;
	}

	int CallbackConicTo(const FT_Vector* a_Control, const FT_Vector* a_To, void* a_User)
	{
		GlyphOutline* outline = (GlyphOutline*)a_User;
		if (outline == nullptr || outline->contours.size() == 0)
		{
			return -1;
		}

		GlyphContour* contour = outline->contours.back();
		if (contour->points.size() == 0)
		{
			return -1;
		}

		glm::vec2 a = contour->points.back();
		glm::vec2 b = Fixed26Dot6::ToFloatVec2(a_To);
		glm::vec2 c = Fixed26Dot6::ToFloatVec2(a_Control);

		int precision = 10;
		glm::vec2 delta_precision((float)precision, (float)precision);

		glm::vec2 delta_ac = (c - a) / delta_precision;
		glm::vec2 delta_cb = (b - c) / delta_precision;

		glm::vec2 start = a;

		for (int j = 1; j < precision; ++j)
		{
			a += delta_ac;
			c += delta_cb;

			glm::vec2 end = a + ((c - a) / delta_precision) * (float)j;

			contour->points.push_back(end);

			start = end;
		}

		contour->points.push_back(b);

		return 0;
	}

	int CallbackCubicTo(const FT_Vector* a_ControlA, const FT_Vector* a_ControlB, const FT_Vector* a_To, void* a_User)
	{
		GlyphOutline* outline = (GlyphOutline*)a_User;
		if (outline == nullptr || outline->contours.size() == 0)
		{
			return -1;
		}

		GlyphContour* contour = outline->contours.back();
		if (contour->points.size() == 0)
		{
			return -1;
		}

		glm::vec2 a = contour->points.back();
		glm::vec2 b = Fixed26Dot6::ToFloatVec2(a_To);
		glm::vec2 c = Fixed26Dot6::ToFloatVec2(a_ControlA);
		glm::vec2 d = Fixed26Dot6::ToFloatVec2(a_ControlB);

		int precision = 10;
		glm::vec2 delta_precision((float)precision, (float)precision);

		glm::vec2 delta_ac = (c - a) / delta_precision;
		glm::vec2 delta_cd = (d - c) / delta_precision;
		glm::vec2 delta_db = (b - d) / delta_precision;

		glm::vec2 start = a;

		for (int j = 1; j < precision; ++j)
		{
			a += delta_ac;
			c += delta_cd;
			d += delta_db;

			glm::vec2 ac = a + ((c - a) / delta_precision) * (float)j;
			glm::vec2 cd = c + ((d - c) / delta_precision) * (float)j;

			glm::vec2 end = ac + ((cd - ac) / delta_precision) * (float)j;

			contour->points.push_back(end);

			start = end;
		}

		contour->points.push_back(b);

		return 0;
	}

	FontFreetype::FontFreetype(const std::string& a_Family)
		: IFont(a_Family)
		, m_Font(nullptr)
	{
		m_OutlineCallbacks.move_to = &CallbackMoveTo;
		m_OutlineCallbacks.line_to = &CallbackLineTo;
		m_OutlineCallbacks.conic_to = &CallbackConicTo;
		m_OutlineCallbacks.cubic_to = &CallbackCubicTo;
		m_OutlineCallbacks.shift = 0;
		m_OutlineCallbacks.delta = 0;
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
				_LoadOutline(m_Font->glyph, glyph);

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
		if (a_Slot->outline.n_contours <= 0)
		{
			return false;
		}

		FT_Error errors = 0;

		a_Glyph->outline = new GlyphOutline;

		errors = FT_Outline_Decompose(&a_Slot->outline, &m_OutlineCallbacks, a_Glyph->outline);
		if (errors != 0)
		{
			delete a_Glyph->outline;
			a_Glyph->outline = nullptr;

			return false;
		}

		return true;
	}

}; // namespace ExLibris