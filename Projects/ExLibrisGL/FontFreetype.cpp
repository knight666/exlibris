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

#include "FontFreetype.h"

#include "CurvePath.h"
#include "FontFace.h"
#include "FreetypeConversion.h"
#include "Glyph.h"
#include "PolygonShape.h"

namespace ExLibris
{

	int CurvePathMoveTo(const FT_Vector* a_To, void* a_User)
	{
		CurvePath* path = (CurvePath*)a_User;

		path->Move(Fixed26Dot6::ToFloatVec2(a_To));

		return 0;
	}

	int CurvePathLineTo(const FT_Vector* a_To, void* a_User)
	{
		CurvePath* path = (CurvePath*)a_User;

		path->LineTo(Fixed26Dot6::ToFloatVec2(a_To));

		return 0;
	}

	int CurvePathConicTo(const FT_Vector* a_Control, const FT_Vector* a_To, void* a_User)
	{
		CurvePath* path = (CurvePath*)a_User;

		path->ConicCurveTo(
			Fixed26Dot6::ToFloatVec2(a_Control),
			Fixed26Dot6::ToFloatVec2(a_To)
		);

		return 0;
	}

	int CurvePathCubicTo(const FT_Vector* a_ControlA, const FT_Vector* a_ControlB, const FT_Vector* a_To, void* a_User)
	{
		CurvePath* path = (CurvePath*)a_User;

		path->QuadraticCurveTo(
			Fixed26Dot6::ToFloatVec2(a_ControlA),
			Fixed26Dot6::ToFloatVec2(a_ControlB),
			Fixed26Dot6::ToFloatVec2(a_To)
		);

		return 0;
	}

	FontFreetype::FontFreetype(Family* a_Family)
		: IFont(a_Family)
		, m_Font(nullptr)
		, m_Buffer(nullptr)
		, m_BufferSize(0)
	{
		m_OutlineCallbacks.move_to = &CurvePathMoveTo;
		m_OutlineCallbacks.line_to = &CurvePathLineTo;
		m_OutlineCallbacks.conic_to = &CurvePathConicTo;
		m_OutlineCallbacks.cubic_to = &CurvePathCubicTo;
		m_OutlineCallbacks.shift = 0;
		m_OutlineCallbacks.delta = 0;
	}
	
	FontFreetype::~FontFreetype()
	{
		if (m_Font != nullptr)
		{
			FT_Done_Face(m_Font);
		}

		if (m_Buffer != nullptr)
		{
			delete [] m_Buffer;
			m_Buffer = nullptr;
		}

		for (std::map<float, FontFace*>::iterator face_it = m_Faces.begin(); face_it != m_Faces.end(); ++face_it)
		{
			delete face_it->second;
		}
		m_Faces.clear();
	}

	unsigned int FontFreetype::GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const
	{
		if (m_Font == nullptr)
		{
			return 0;
		}

		return (unsigned int)FT_Get_Char_Index(m_Font, (FT_ULong)a_CodepointUtf32);
	}

	void FontFreetype::SetFontData(FT_Face a_Font, FT_Byte* a_Buffer, size_t a_BufferSize)
	{
		m_Font = a_Font;
		m_Buffer = a_Buffer;
		m_BufferSize = a_BufferSize;
	}

	FontFace* FontFreetype::CreateFace(const FaceOptions& a_Options)
	{
		std::map<float, FontFace*>::iterator found = m_Faces.find(a_Options.size);
		if (found != m_Faces.end())
		{
			return found->second;
		}

		if (m_Font == nullptr)
		{
			return nullptr;
		}

		FT_Error errors = 0;

		errors = FT_Set_Char_Size(m_Font, 0, Fixed26Dot6::ToFixed(a_Options.size), 0, 96);
		if (errors != 0)
		{
			return nullptr;
		}

		FontFace* face = new FontFace(this);
		face->SetSize(a_Options.size);
		face->SetLineHeight(Fixed26Dot6::ToFloat(m_Font->size->metrics.height));
		face->SetAscender(Fixed26Dot6::ToFloat(m_Font->size->metrics.ascender));
		face->SetDescender(Fixed26Dot6::ToFloat(m_Font->size->metrics.descender));

		std::vector<Glyph*> glyphs;

		FT_UInt codepoint = 0;
		FT_ULong glyph_index = FT_Get_First_Char(m_Font, &codepoint);
		do
		{
			errors = FT_Load_Glyph(m_Font, codepoint, FT_LOAD_DEFAULT);
			if (errors == 0)
			{
				Glyph* glyph = new Glyph;
				glyph->index = (unsigned int)codepoint;

				_LoadMetrics(m_Font->glyph, glyph, m_Font->size->metrics);

				if (a_Options.bitmap_quality == eBitmapQuality_Color)
				{
					_LoadBitmapColor(m_Font->glyph, glyph);
				}
				else if (a_Options.bitmap_quality == eBitmapQuality_Mono)
				{
					_LoadBitmapMono(m_Font->glyph, glyph);
				}

				if (a_Options.outline)
				{
					_LoadOutline(m_Font->glyph, glyph);
				}

				glyphs.push_back(glyph);
			}

			glyph_index = FT_Get_Next_Char(m_Font, glyph_index, &codepoint);
		}
		while (codepoint != 0);

		if (a_Options.kerning)
		{
			for (std::vector<Glyph*>::iterator glyph_left_it = glyphs.begin(); glyph_left_it != glyphs.end(); ++glyph_left_it)
			{
				Glyph* glyph_left = *glyph_left_it;

				if (glyph_left_it + 1 != glyphs.end())
				{
					for (std::vector<Glyph*>::iterator glyph_right_it = glyph_left_it + 1; glyph_right_it != glyphs.end(); ++glyph_right_it)
					{
						Glyph* glyph_right = *glyph_right_it;

						FT_Vector kerning_fixed;

						errors = FT_Get_Kerning(m_Font, glyph_left->index, glyph_right->index, FT_KERNING_DEFAULT, &kerning_fixed);
						if (errors == 0 && (kerning_fixed.x != 0 || kerning_fixed.y != 0))
						{
							glm::vec2 kerning = Fixed26Dot6::ToFloatVec2(&kerning_fixed);

							glyph_left->kernings.insert(std::make_pair(glyph_right->index, kerning));
						}

						errors = FT_Get_Kerning(m_Font, glyph_right->index, glyph_left->index, FT_KERNING_DEFAULT, &kerning_fixed);
						if (errors == 0 && (kerning_fixed.x != 0 || kerning_fixed.y != 0))
						{
							glm::vec2 kerning = Fixed26Dot6::ToFloatVec2(&kerning_fixed);

							glyph_right->kernings.insert(std::make_pair(glyph_left->index, kerning));
						}
					}
				}

				if (!face->AddGlyph(glyph_left))
				{
					delete glyph_left;
				}
			}
		}
		
		m_Faces.insert(std::make_pair(a_Options.size, face));

		return face;
	}

	bool FontFreetype::_LoadMetrics(FT_GlyphSlot a_Slot, Glyph* a_Glyph, FT_Size_Metrics a_FontMetrics) const
	{
		FT_Error errors = 0;

		FT_Glyph_Metrics& glyph_metrics = a_Slot->metrics;

		a_Glyph->metrics = new GlyphMetrics;
		a_Glyph->metrics->offset.x = Fixed26Dot6::ToFloat(glyph_metrics.horiBearingX);
		a_Glyph->metrics->offset.y = Fixed26Dot6::ToFloat(a_FontMetrics.height - glyph_metrics.horiBearingY);
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

	bool FontFreetype::_LoadBitmapColor(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const
	{
		FT_Error errors = 0;

		errors = FT_Render_Glyph(a_Slot, FT_RENDER_MODE_NORMAL);
		if (errors != 0)
		{
			return false;
		}

		FT_Bitmap& glyph_bitmap = a_Slot->bitmap;

		unsigned int bitmap_size = glyph_bitmap.width * glyph_bitmap.rows * 4;
		if (bitmap_size == 0)
		{
			return false;
		}

		a_Glyph->bitmap = new GlyphBitmap;
		a_Glyph->bitmap->width = glyph_bitmap.width;
		a_Glyph->bitmap->height = glyph_bitmap.rows;
		a_Glyph->bitmap->data = new unsigned char[bitmap_size];

		unsigned char* src_line = glyph_bitmap.buffer;
		unsigned int src_pitch = glyph_bitmap.pitch;

		unsigned char* dst_line = a_Glyph->bitmap->data;
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

		return true;
	}

	bool FontFreetype::_LoadBitmapMono(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const
	{
		FT_Error errors = 0;

		errors = FT_Render_Glyph(a_Slot, FT_RENDER_MODE_MONO);
		if (errors != 0)
		{
			return false;
		}

		FT_Bitmap& glyph_bitmap = a_Slot->bitmap;

		unsigned int bitmap_size = glyph_bitmap.width * glyph_bitmap.rows * 4;
		if (bitmap_size == 0)
		{
			return false;
		}

		a_Glyph->bitmap = new GlyphBitmap;
		a_Glyph->bitmap->width = glyph_bitmap.width;
		a_Glyph->bitmap->height = glyph_bitmap.rows;
		a_Glyph->bitmap->data = new unsigned char[bitmap_size];

		unsigned char* src_line = glyph_bitmap.buffer;
		unsigned int src_pitch = glyph_bitmap.pitch;

		unsigned char* dst_line = a_Glyph->bitmap->data;
		unsigned int dst_pitch = glyph_bitmap.width * 4;

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

		return true;
	}

	bool FontFreetype::_LoadOutline(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const
	{
		if (a_Slot->outline.n_contours <= 0)
		{
			return false;
		}

		FT_Error errors = 0;

		FT_Matrix transform_flipped;
		transform_flipped.xx = Fixed16Dot16::ToFixed(1.0f);
		transform_flipped.xy = 0;
		transform_flipped.yx = 0;
		transform_flipped.yy = Fixed16Dot16::ToFixed(-1.0f);

		FT_Outline_Transform(&a_Slot->outline, &transform_flipped);

		a_Glyph->outline = new CurvePath;

		errors = FT_Outline_Decompose(&a_Slot->outline, &m_OutlineCallbacks, a_Glyph->outline);
		if (errors != 0)
		{
			return false;
		}

		return true;
	}

}; // namespace ExLibris