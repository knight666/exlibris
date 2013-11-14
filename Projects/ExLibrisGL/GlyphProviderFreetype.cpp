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
#include "GlyphMetrics.h"

namespace ExLibris
{

	GlyphProviderFreetype::GlyphProviderFreetype(FT_Face a_Face, FT_Byte* a_Buffer, size_t a_BufferSize)
		: m_Face(a_Face)
		, m_Buffer(a_Buffer)
		, m_BufferSize(a_BufferSize)
		, m_FaceSizeLoaded(0)
		, m_GlyphLoaded(-1)
	{
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

	GlyphMetrics* GlyphProviderFreetype::CreateMetrics(const GlyphRequest& a_Request)
	{
		if (!_SetFaceSize(a_Request.size) || !_LoadGlyph(a_Request.codepoint))
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

	bool GlyphProviderFreetype::TryGetKerningAdjustment(glm::vec2& a_Kerning, const GlyphRequest& a_Request, int a_CodepointNext)
	{
		if (!FT_HAS_KERNING(m_Face) || !_SetFaceSize(a_Request.size))
		{
			return false;
		}

		FT_Error errors = 0;

		FT_UInt index_current = FT_Get_Char_Index(m_Face, (FT_ULong)a_Request.codepoint);
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

	GlyphBitmap* GlyphProviderFreetype::CreateBitmap(const GlyphRequest& a_Request)
	{
		if (!_SetFaceSize(a_Request.size) || !_LoadGlyph(a_Request.codepoint))
		{
			return nullptr;
		}

		FT_Error errors = 0;

		return nullptr;
	}

	CurvePath* GlyphProviderFreetype::CreateOutline(const GlyphRequest& a_Request)
	{
		if (!_SetFaceSize(a_Request.size) || !_LoadGlyph(a_Request.codepoint))
		{
			return nullptr;
		}

		FT_Error errors = 0;

		return nullptr;
	}

	bool GlyphProviderFreetype::_SetFaceSize(float a_Size)
	{
		FT_Error errors = 0;

		FT_F26Dot6 requested_size = Fixed26Dot6::ToFixed(a_Size);
		if (requested_size != m_FaceSizeLoaded)
		{
			errors = FT_Set_Char_Size(m_Face, 0, requested_size, 0, 96);
			if (errors != FT_Err_Ok)
			{
				EXL_FT_THROW("FontProviderFreetype::_SetFaceSize", errors);

				return false;
			}

			m_FaceSizeLoaded = requested_size;
		}

		return true;
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