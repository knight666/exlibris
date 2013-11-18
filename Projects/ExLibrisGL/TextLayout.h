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

#include "Alignment.h"
#include "Face.h"
#include "ITextLayoutVisitor.h"
#include "SizePolicy.h"
#include "TextCharacter.h"
#include "TextLine.h"

namespace ExLibris
{

	class TextLayout
	{
	
	public:

		enum WordWrapping
		{
			eWordWrapping_None,
			eWordWrapping_Greedy
		};

	public:
	
		TextLayout();
		~TextLayout();

		Face* GetFace() const;
		void SetFace(Face* a_Face);

		const std::string& GetText() const;
		void SetText(const std::string& a_Text);

		const glm::vec2& GetSizeHint() const;
		void SetSizeHint(const glm::vec2& a_SizeHint);

		void SetHorizontalSizePolicy(SizePolicy a_Policy);
		void SetVerticalSizePolicy(SizePolicy a_Policy);

		void SetHorizontalAlignment(HorizontalAlignment a_Alignment);
		void SetVerticalAlignment(VerticalAlignment a_Alignment);

		void SetWordWrapping(WordWrapping a_Wrapping);

		const glm::vec2& GetDimensions() const;

		const BoundingBox& GetBoundingBox() const;

		void Layout();

		std::vector<TextLine*> GetLines() const;

		void Accept(ITextLayoutVisitor& a_Visitor);

	private:

		std::vector<int> _AsciiToUtf32(const std::string& a_Text);
		void _ConvertTextToGlyphs();
		void _ConvertGlyphsToLines();
		void _WordWrappingNone(bool a_WidthFixed, bool a_HeightFixed);
		void _WordWrappingGreedy(bool a_WidthFixed, bool a_HeightFixed);
		void _CalculateDimensions();
		void _LayoutLines();

		bool _NextLine(bool a_HeightFixed);
		void _AddGlyphToCurrentLine(TextCharacter* a_Glyph);

	private:

		Face* m_Face;
		std::string m_Text;

		std::vector<TextCharacter*> m_Glyphs;
		bool m_GlyphsDirty;

		TextCharacter* m_GlyphSpace;
		GlyphMetrics* m_MetricsSpace;

		std::vector<TextLine*> m_Lines;
		TextLine* m_LineCurrent;
		bool m_LinesDirty;

		glm::vec2 m_Cursor;
		glm::vec2 m_Dimensions;
		BoundingBox m_BoundingBox;

		glm::vec2 m_SizeHint;
		HorizontalAlignment m_HorizontalAlignment;
		VerticalAlignment m_VerticalAlignment;
		WordWrapping m_WordWrapping;
		SizePolicy m_HorizontalPolicy;
		SizePolicy m_VerticalPolicy;
		bool m_LayoutDirty;
	
	}; // class TextLayout

}; // namespace ExLibris