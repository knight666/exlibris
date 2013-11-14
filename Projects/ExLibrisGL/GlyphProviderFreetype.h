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

#include "FontLoaderFreetype.h"
#include "IGlyphProvider.h"

namespace ExLibris
{

	class GlyphProviderFreetype
		: IGlyphProvider
	{
	
	public:
	
		GlyphProviderFreetype(FT_Face a_Face, FT_Byte* a_Buffer, size_t a_BufferSize);
		~GlyphProviderFreetype();
	
		GlyphMetrics* CreateMetrics(const GlyphRequest& a_Request);

		bool TryGetKerningAdjustment(glm::vec2& a_Kerning, const GlyphRequest& a_Request, int a_CodepointNext);

		GlyphBitmap* CreateBitmap(const GlyphRequest& a_Request);

		CurvePath* CreateOutline(const GlyphRequest& a_Request);

	private:

		bool _SetFaceSize(float a_Size);
		bool _LoadGlyph(int a_Codepoint);

	private:

		FT_Face m_Face;
		FT_F26Dot6 m_FaceSizeLoaded;
		FT_UInt m_GlyphLoaded;

		FT_Byte* m_Buffer;
		size_t m_BufferSize;
	
	}; // class GlyphProviderFreetype

}; // namespace ExLibris