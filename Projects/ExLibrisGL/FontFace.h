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

#include <map>

#include "Glyph.h"

namespace ExLibris
{
	class Family;
	class IFont;
}

namespace ExLibris
{

	class FontFace
	{
	
	public:
	
		FontFace(const IFont* a_Font);
		~FontFace();
	
		const IFont* GetFont() const;

		Family* GetFamily() const;

		float GetSize() const;
		void SetSize(float a_Size);

		float GetLineHeight() const;
		void SetLineHeight(float a_LineHeight);

		float GetAscender() const;
		void SetAscender(float a_Ascender);

		float GetDescender() const;
		void SetDescender(float a_Descender);

		size_t GetGlyphCount() const;

		bool AddGlyph(Glyph* a_Glyph);
		Glyph* FindGlyph(unsigned int a_CodepointUtf32) const;

		bool TryGetKerning(Glyph* a_Left, Glyph* a_Right, glm::vec2& a_Target) const;

	private:

		const IFont* m_Font;
		float m_Size;
		float m_LineHeight;
		float m_Ascender;
		float m_Descender;
		std::map<unsigned int, Glyph*> m_Glyphs;
	
	}; // class FontFace

}; // namespace ExLibris