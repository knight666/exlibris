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

#include "FontLoaderFreetype.h"

#include "FreetypeErrors.h"
#include "GlyphProviderFreetype.h"
#include "Library.h"

namespace ExLibris
{

	FontLoaderFreetype::FontLoaderFreetype(Library* a_Library)
		: IFontLoader(a_Library)
	{
		FT_Error errors = 0;
		
		errors = FT_Init_FreeType(&m_FreetypeLibrary);
		if (errors != FT_Err_Ok)
		{
			EXL_FT_THROW("FontLoaderFreetype::FontLoaderFreetype", errors);
		}
	}

	FontLoaderFreetype::~FontLoaderFreetype()
	{
		FT_Error errors = 0;

		errors = FT_Done_FreeType(m_FreetypeLibrary);
		if (errors != FT_Err_Ok)
		{
			EXL_FT_THROW("FontLoaderFreetype::~FontLoaderFreetype", errors);
		}
	}

	FT_Library FontLoaderFreetype::GetFreetypeLibrary() const
	{
		return m_FreetypeLibrary;
	}

	IFont* FontLoaderFreetype::LoadFont(std::istream& a_Stream)
	{
		a_Stream.seekg(0, std::ios_base::end);
		std::streamoff font_file_size = a_Stream.tellg();
		a_Stream.seekg(0, std::ios_base::beg);

		if (font_file_size <= 0)
		{
			return nullptr;
		}

		FT_Byte* font_file_data = new FT_Byte[(unsigned int)font_file_size];
		a_Stream.read((char*)font_file_data, font_file_size);

		FT_Error errors = 0;

		FT_Face font_loaded = nullptr;
		errors = FT_New_Memory_Face(m_FreetypeLibrary, font_file_data, (FT_Long)font_file_size, 0, &font_loaded);
		if (errors != FT_Err_Ok)
		{
			EXL_FT_THROW("FontLoaderFreetype::LoadFont", errors);

			return nullptr;
		}

		errors = FT_Select_Charmap(font_loaded, FT_ENCODING_UNICODE);
		if (errors != FT_Err_Ok)
		{
			EXL_FT_THROW("FontLoaderFreetype::LoadFont", errors);

			return nullptr;
		}

		if (font_loaded->charmap == nullptr)
		{
			EXL_THROW("FontLoaderFreetype::LoadFont", "Font does not have a unicode charmap.");

			return nullptr;
		}

		Family* family = m_Library->CreateFamily(font_loaded->family_name);

		FontFreetype* font = new FontFreetype(family);

		font->SetFontData(font_loaded, font_file_data, (size_t)font_file_size);

		font->SetWeight(((font_loaded->style_flags & FT_STYLE_FLAG_BOLD) != 0) ? eWeight_Bold : eWeight_Normal);
		font->SetStyle(((font_loaded->style_flags & FT_STYLE_FLAG_ITALIC) != 0) ? eStyle_Italicized : eStyle_None);

		return font;
	}

	IGlyphProvider* FontLoaderFreetype::LoadGlyphProvider(std::istream& a_Stream)
	{
		a_Stream.seekg(0, std::ios_base::end);
		std::streamoff font_file_size = a_Stream.tellg();
		a_Stream.seekg(0, std::ios_base::beg);

		if (font_file_size <= 0)
		{
			return nullptr;
		}

		FT_Byte* font_file_data = new FT_Byte[(unsigned int)font_file_size];
		a_Stream.read((char*)font_file_data, font_file_size);

		FT_Error errors = 0;

		FT_Face face_loaded = nullptr;
		errors = FT_New_Memory_Face(m_FreetypeLibrary, font_file_data, (FT_Long)font_file_size, 0, &face_loaded);
		if (errors != FT_Err_Ok)
		{
			EXL_FT_THROW("FontLoaderFreetype::LoadGlyphProvider", errors);

			return nullptr;
		}

		GlyphProviderFreetype* provider = new GlyphProviderFreetype(m_Library, face_loaded, font_file_data, (size_t)font_file_size);

		return provider;
	}

}; // namespace ExLibris