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

#include "CurvePath.h"
#include "Face.h"
#include "FreetypeConversion.h"
#include "GlyphBitmap.h"
#include "GlyphMetrics.h"
#include "Library.h"

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

	GlyphProviderFreetype::GlyphProviderFreetype(Library* a_Library, FT_Face a_Face, FT_Byte* a_Buffer, size_t a_BufferSize)
		: IGlyphProvider(a_Library)
		, m_Face(a_Face)
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

		m_Family = m_Library->CreateFamily(m_Face->family_name);

		m_Weight = ((m_Face->style_flags & FT_STYLE_FLAG_BOLD) != 0) ? eWeight_Bold : eWeight_Normal;
		m_Style = ((m_Face->style_flags & FT_STYLE_FLAG_ITALIC) != 0) ? eStyle_Italicized : eStyle_None;

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

	bool GlyphProviderFreetype::HasKerning() const
	{
		return (FT_HAS_KERNING(m_Face) != 0);
	}

	bool GlyphProviderFreetype::IsScalable() const
	{
		return (FT_IS_SCALABLE(m_Face) != 0);
	}

	bool GlyphProviderFreetype::IsSizeAvailable(float a_Size) const
	{
		if (FT_HAS_FIXED_SIZES(m_Face) == 0)
		{
			return true;
		}
		else
		{
			FT_Pos requested_size = (FT_Pos)Fixed26Dot6::ToFixed(a_Size);

			for (FT_Int i = 0; i < m_Face->num_fixed_sizes; ++i)
			{
				if (m_Face->available_sizes[i].y_ppem == requested_size)
				{
					return true;
				}
			}
			
			return false;
		}
	}

	unsigned int GlyphProviderFreetype::GetIndexForCodepoint(int a_CodepointUtf32)
	{
		unsigned int index = (unsigned int)FT_Get_Char_Index(m_Face, (FT_ULong)a_CodepointUtf32);
		
		return ((index == 0) ? EXL_INVALID_INDEX : index);
	}

	GlyphMetrics* GlyphProviderFreetype::CreateMetrics(float a_Size, unsigned int a_Index)
	{
		if (EXL_IS_INDEX_INVALID(a_Index) || !_SetSize(a_Size) || !_LoadGlyph(a_Index))
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

		if (slot->format == FT_GLYPH_FORMAT_OUTLINE || slot->format == FT_GLYPH_FORMAT_COMPOSITE)
		{
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
		}
		else
		{
			metrics->bounding_box = BoundingBox(
				glm::vec2(
					(float)(slot->bitmap_left),
					(float)(slot->bitmap.rows - slot->bitmap_top)
				),
				glm::vec2(
					(float)(slot->bitmap_left + slot->bitmap.width),
					(float)(slot->bitmap.rows - slot->bitmap_top + slot->bitmap_top)
				)
			);
		}

		return metrics;
	}

	GlyphBitmap* GlyphProviderFreetype::CreateBitmap(float a_Size, unsigned int a_Index)
	{
		if (EXL_IS_INDEX_INVALID(a_Index) || !_SetSize(a_Size) || !_LoadGlyph(a_Index))
		{
			return nullptr;
		}

		FT_Error errors = 0;

		errors = FT_Render_Glyph(m_Face->glyph, FT_RENDER_MODE_NORMAL);
		if (errors != FT_Err_Ok)
		{
			EXL_FT_THROW("GlyphProviderFreetype::CreateBitmap", errors);

			return nullptr;
		}

		FT_Bitmap& glyph_bitmap = m_Face->glyph->bitmap;
	
		unsigned int bitmap_size = glyph_bitmap.width * glyph_bitmap.rows * 4;
		if (bitmap_size == 0)
		{
			return nullptr;
		}

		GlyphBitmap* bitmap = new GlyphBitmap;
		bitmap->width = glyph_bitmap.width;
		bitmap->height = glyph_bitmap.rows;
		bitmap->data = new unsigned char[bitmap_size];

		unsigned char* src_line = glyph_bitmap.buffer;
		unsigned int src_pitch = glyph_bitmap.pitch;

		unsigned char* dst_line = bitmap->data;
		unsigned int dst_pitch = glyph_bitmap.width * 4;

		if (glyph_bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
		{
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
		}
		else if (glyph_bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
		{
			for (int y = 0; y < glyph_bitmap.rows; ++y)
			{
				int mask = 0x80;

				unsigned char* src = src_line;
				unsigned char* dst = dst_line;

				for (int x = 0; x < glyph_bitmap.width; ++x)
				{
					char value = ((*src & mask) == mask) ? 0xFF : 0x00;
					dst[0] = value;
					dst[1] = value;
					dst[2] = value;
					dst[3] = value;

					dst += 4;

					mask >>= 1;
					if (mask <= 0)
					{
						mask = 0x80;
						src++;
					}
				}

				src_line += src_pitch;
				dst_line += dst_pitch;
			}
		}
		else
		{
			StringStream ss;
			ss << "Unhandled bitmap type: " << glyph_bitmap.pixel_mode << std::endl;
			EXL_THROW("GlyphProviderFreetype::CreateBitmap", ss.str().c_str());

			return nullptr;
		}

		return bitmap;
	}

	CurvePath* GlyphProviderFreetype::CreateOutline(float a_Size, unsigned int a_Index)
	{
		if (EXL_IS_INDEX_INVALID(a_Index) || !IsScalable() || !_SetSize(a_Size) || !_LoadGlyph(a_Index))
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

	bool GlyphProviderFreetype::TryGetKerningAdjustment(glm::vec2& a_Adjustment, float a_Size, unsigned int a_IndexCurrent, unsigned int a_IndexNext)
	{
		if (EXL_IS_INDEX_INVALID(a_IndexCurrent) || EXL_IS_INDEX_INVALID(a_IndexNext) || !HasKerning() || !_SetSize(a_Size))
		{
			return false;
		}

		FT_Error errors = 0;

		FT_Vector kerning_fixed;

		errors = FT_Get_Kerning(
			m_Face,
			(FT_UInt)a_IndexCurrent,
			(FT_UInt)a_IndexNext,
			FT_KERNING_DEFAULT,
			&kerning_fixed
		);
		if (errors != FT_Err_Ok)
		{
			return false;
		}

		if (kerning_fixed.x != 0 || kerning_fixed.y != 0)
		{
			a_Adjustment = Fixed26Dot6::ToFloatVec2(&kerning_fixed);

			return true;
		}
		else
		{
			return false;
		}
	}

	Face* GlyphProviderFreetype::CreateFace(float a_Size)
	{
		if (!_SetSize(a_Size))
		{
			return nullptr;
		}

		FontMetrics metrics;
		metrics.family = m_Family;
		metrics.weight = m_Weight;
		metrics.style = m_Style;
		metrics.size = a_Size;
		metrics.line_height = Fixed26Dot6::ToFloat(m_Face->size->metrics.height);
		metrics.ascent = Fixed26Dot6::ToFloat(m_Face->size->metrics.ascender);
		metrics.descent = Fixed26Dot6::ToFloat(m_Face->size->metrics.descender);

		return new Face(*this, metrics);
	}

	bool GlyphProviderFreetype::_SetSize(float a_Size)
	{
		FT_Error errors = 0;

		FT_F26Dot6 requested_size = Fixed26Dot6::ToFixed(a_Size);
		if (requested_size != m_SizeLoaded)
		{
			if (IsScalable())
			{
				errors = FT_Set_Char_Size(m_Face, 0, requested_size, 0, 96);
			}
			else
			{
				FT_UInt pixel_size = (FT_UInt)a_Size;

				errors = FT_Set_Pixel_Sizes(m_Face, pixel_size, pixel_size);
			}

			if (errors != FT_Err_Ok)
			{
				EXL_FT_THROW("FontProviderFreetype::_SetSize", errors);

				return false;
			}

			m_SizeLoaded = requested_size;
		}

		return true;
	}

	bool GlyphProviderFreetype::_LoadGlyph(unsigned int a_Index)
	{
		FT_Error errors = 0;

		if (a_Index != m_GlyphLoaded)
		{
			errors = FT_Load_Glyph(m_Face, (FT_UInt)a_Index, FT_LOAD_DEFAULT);
			if (errors != FT_Err_Ok)
			{
				EXL_FT_THROW("FontProviderFreetype::_LoadGlyph", errors);

				return false;
			}

			m_GlyphLoaded = (FT_UInt)a_Index;
		}

		return true;
	}

}; // namespace ExLibris