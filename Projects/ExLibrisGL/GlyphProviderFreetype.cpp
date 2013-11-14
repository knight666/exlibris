/*
 * This file is a part of the ExLibris project.
 *
 * Copyright (C) 2013 Quinten Lansu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

#include "ExLibrisGL.PCH.h"

#include "GlyphProviderFreetype.h"

#include "FreetypeConversion.h"
#include "CurvePath.h"
#include "GlyphBitmap.h"
#include "GlyphMetrics.h"

namespace ExLibris
{

	struct GlyphTarget
	{
		CurvePath* outline;
		FT_BBox bounding_box;
	};

	inline glm::vec2 GetCurvePosition(const FT_Vector* a_Position, GlyphTarget* a_Target)
	{
		return glm::vec2(
			Fixed26Dot6::ToFloat(a_Position->x - a_Target->bounding_box.xMin), 
			Fixed26Dot6::ToFloat(a_Target->bounding_box.yMax - a_Position->y)
		);
	}

	int OutlineMoveTo(const FT_Vector* a_To, void* a_User)
	{
		GlyphTarget* target = (GlyphTarget*)a_User;

		target->outline->Move(GetCurvePosition(a_To, target));

		return 0;
	}

	int OutlineLineTo(const FT_Vector* a_To, void* a_User)
	{
		GlyphTarget* target = (GlyphTarget*)a_User;

		target->outline->LineTo(GetCurvePosition(a_To, target));

		return 0;
	}

	int OutlineConicTo(const FT_Vector* a_Control, const FT_Vector* a_To, void* a_User)
	{
		GlyphTarget* target = (GlyphTarget*)a_User;

		target->outline->ConicCurveTo(
			GetCurvePosition(a_Control, target),
			GetCurvePosition(a_To, target)
		);

		return 0;
	}

	int OutlineCubicTo(const FT_Vector* a_ControlA, const FT_Vector* a_ControlB, const FT_Vector* a_To, void* a_User)
	{
		GlyphTarget* target = (GlyphTarget*)a_User;

		target->outline->QuadraticCurveTo(
			GetCurvePosition(a_ControlA, target),
			GetCurvePosition(a_ControlB, target),
			GetCurvePosition(a_To, target)
		);

		return 0;
	}

	GlyphProviderFreetype::GlyphProviderFreetype(FT_Face a_Face, FT_Byte* a_Buffer, size_t a_BufferSize)
		: m_Face(a_Face)
		, m_Buffer(a_Buffer)
		, m_BufferSize(a_BufferSize)
		, m_SizeLoaded(0)
		, m_GlyphLoaded(-1)
	{
		if (m_Face == nullptr || m_Buffer == nullptr)
		{
			EXL_THROW("GlyphProviderFreetype::GlyphProviderFreetype", "Provider must have a valid face handle.");
			return;
		}

		m_OutlineCallbacks.move_to = &OutlineMoveTo;
		m_OutlineCallbacks.line_to = &OutlineLineTo;
		m_OutlineCallbacks.conic_to = &OutlineConicTo;
		m_OutlineCallbacks.cubic_to = &OutlineCubicTo;
		m_OutlineCallbacks.shift = 0;
		m_OutlineCallbacks.delta = 0;
	}
	
	GlyphProviderFreetype::~GlyphProviderFreetype()
	{
		if (m_Face != nullptr)
		{
			FT_Done_Face(m_Face);
		}

		if (m_Buffer != nullptr)
		{
			delete [] m_Buffer;
			m_Buffer = nullptr;
		}
	}

	bool GlyphProviderFreetype::SetSize(float a_Size)
	{
		FT_Error errors = 0;

		FT_F26Dot6 requested_size = Fixed26Dot6::ToFixed(a_Size);
		if (requested_size != m_SizeLoaded)
		{
			errors = FT_Set_Char_Size(m_Face, 0, requested_size, 0, 96);
			if (errors != FT_Err_Ok)
			{
				EXL_FT_THROW("FontProviderFreetype::SetSize", errors);

				return false;
			}

			m_SizeLoaded = requested_size;
		}

		return true;
	}

	GlyphMetrics* GlyphProviderFreetype::CreateMetrics(int a_Codepoint)
	{
		if (!_LoadGlyph(a_Codepoint))
		{
			return nullptr;
		}

		FT_Error errors = 0;

		FT_GlyphSlot slot = m_Face->glyph;
		FT_Glyph_Metrics glyph_metrics = slot->metrics;
		FT_Size_Metrics face_metrics = m_Face->size->metrics;

		GlyphMetrics* metrics = new GlyphMetrics;
		metrics->offset.x = Fixed26Dot6::ToFloat(glyph_metrics.horiBearingX);
		metrics->offset.y = Fixed26Dot6::ToFloat(face_metrics.height - glyph_metrics.horiBearingY - face_metrics.ascender + face_metrics.descender);
		metrics->advance = Fixed26Dot6::ToFloat(glyph_metrics.horiAdvance);

		// bounding box

		FT_BBox bounding_box;
		errors = FT_Outline_Get_BBox(&slot->outline, &bounding_box);
		if (errors == FT_Err_Ok)
		{
			metrics->bounding_box = BoundingBox(
				glm::vec2(
					Fixed26Dot6::ToFloat(bounding_box.xMin),
					Fixed26Dot6::ToFloat(face_metrics.height - bounding_box.yMin)
				),
				glm::vec2(
					Fixed26Dot6::ToFloat(bounding_box.xMax),
					Fixed26Dot6::ToFloat(face_metrics.height - bounding_box.yMax)
				)
			);
		}

		return metrics;
	}

	bool GlyphProviderFreetype::TryGetKerningAdjustment(glm::vec2& a_Kerning, int a_CodepointCurrent, int a_CodepointNext)
	{
		if (!FT_HAS_KERNING(m_Face))
		{
			return false;
		}

		FT_Error errors = 0;

		FT_UInt index_current = FT_Get_Char_Index(m_Face, (FT_ULong)a_CodepointCurrent);
		FT_UInt index_next = FT_Get_Char_Index(m_Face, (FT_ULong)a_CodepointNext);

		if (index_current == 0 || index_next == 0)
		{
			return false;
		}

		FT_Vector kerning_fixed;

		errors = FT_Get_Kerning(m_Face, index_current, index_next, FT_KERNING_DEFAULT, &kerning_fixed);
		if (errors != FT_Err_Ok)
		{
			return false;
		}

		a_Kerning = Fixed26Dot6::ToFloatVec2(&kerning_fixed);

		return true;
	}

	GlyphBitmap* GlyphProviderFreetype::CreateBitmap(int a_Codepoint)
	{
		if (!_LoadGlyph(a_Codepoint))
		{
			return nullptr;
		}

		FT_Error errors = 0;

		errors = FT_Render_Glyph(m_Face->glyph, FT_RENDER_MODE_NORMAL);
		if (errors != FT_Err_Ok)
		{
			EXL_FT_THROW("GlyphProviderFreetype::CreateBitmap", errors);

			return false;
		}

		FT_Bitmap& glyph_bitmap = m_Face->glyph->bitmap;

		unsigned int bitmap_size = glyph_bitmap.width * glyph_bitmap.rows * 4;
		if (bitmap_size == 0)
		{
			return false;
		}

		GlyphBitmap* bitmap = new GlyphBitmap;
		bitmap->width = glyph_bitmap.width;
		bitmap->height = glyph_bitmap.rows;
		bitmap->data = new unsigned char[bitmap_size];

		unsigned char* src_line = glyph_bitmap.buffer;
		unsigned int src_pitch = glyph_bitmap.pitch;

		unsigned char* dst_line = bitmap->data;
		unsigned int dst_pitch = glyph_bitmap.width * 4;

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

		return bitmap;
	}

	CurvePath* GlyphProviderFreetype::CreateOutline(int a_Codepoint)
	{
		if (!_LoadGlyph(a_Codepoint))
		{
			return nullptr;
		}

		if (m_Face->glyph->outline.n_contours <= 0)
		{
			return nullptr;
		}

		FT_Error errors = 0;

		GlyphTarget target;
		target.outline = new CurvePath;

		errors = FT_Outline_Get_BBox(&m_Face->glyph->outline, &target.bounding_box);
		if (errors != FT_Err_Ok)
		{
			EXL_FT_THROW("GlyphProviderFreetype::CreateOutline", errors);

			return nullptr;
		}

		errors = FT_Outline_Decompose(&m_Face->glyph->outline, &m_OutlineCallbacks, &target);
		if (errors != FT_Err_Ok)
		{
			EXL_FT_THROW("GlyphProviderFreetype::CreateOutline", errors);

			return nullptr;
		}

		return target.outline;
	}

	bool GlyphProviderFreetype::_LoadGlyph(int a_Codepoint)
	{
		FT_Error errors = 0;

		FT_UInt requested_index = FT_Get_Char_Index(m_Face, (FT_ULong)a_Codepoint);
		if (requested_index == 0)
		{
			return false;
		}

		if (requested_index != m_GlyphLoaded)
		{
			errors = FT_Load_Glyph(m_Face, requested_index, FT_LOAD_DEFAULT);
			if (errors != FT_Err_Ok)
			{
				EXL_FT_THROW("FontProviderFreetype::_LoadGlyph", errors);

				return false;
			}

			m_GlyphLoaded = requested_index;
		}

		return true;
	}

}; // namespace ExLibris