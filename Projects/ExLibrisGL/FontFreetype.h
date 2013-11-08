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

#pragma once

#include "FreetypeErrors.h"
#include "IFont.h"

namespace ExLibris
{
	struct Glyph;
}

namespace ExLibris
{

	class FontFreetype
		: public IFont
	{
	
	public:
	
		FontFreetype(Family* a_Family);
		~FontFreetype();

		unsigned int GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const;

		void SetFontData(FT_Face a_Font, FT_Byte* a_Buffer, size_t a_BufferSize);

		FontFace* CreateFace(const FaceOptions& a_Options);

	private:

		bool _LoadMetrics(FT_GlyphSlot a_Slot, Glyph* a_Glyph, FT_Size_Metrics a_FontMetrics) const;
		bool _LoadBitmapColor(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const;
		bool _LoadBitmapMono(FT_GlyphSlot a_Slot, Glyph* a_Glyph) const;
		bool _LoadOutline(FT_GlyphSlot a_Slot, Glyph* a_Glyph, FT_Size_Metrics a_FontMetrics) const;

	private:

		FT_Face m_Font;
		FT_Byte* m_Buffer;
		size_t m_BufferSize;

		FT_Outline_Funcs m_OutlineCallbacks;

		std::map<float, FontFace*> m_Faces;
	
	}; // class FontFreetype

}; // namespace ExLibris