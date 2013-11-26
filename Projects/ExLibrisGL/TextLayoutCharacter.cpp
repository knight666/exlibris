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

#include "TextLayoutCharacter.h"

#include "Face.h"
#include "GlyphMetrics.h"

namespace ExLibris
{

	TextLayoutCharacter::TextLayoutCharacter(Face* a_Face, unsigned int a_GlyphIndex, GlyphMetrics* a_Metrics)
		: m_Face(a_Face)
		, m_GlyphIndex(a_GlyphIndex)
		, m_Metrics(a_Metrics)
		, m_Position(0.0f, 0.0f)
		, m_Adjustment(0.0f, 0.0f)
	{
	}
	
	TextLayoutCharacter::~TextLayoutCharacter()
	{
	}

	void TextLayoutCharacter::SetPosition(const glm::vec2& a_Position)
	{
		m_Position = a_Position;
	}

	void TextLayoutCharacter::SetKerningAdjustment(const glm::vec2& a_Adjustment)
	{
		m_Adjustment = a_Adjustment;
	}

	void TextLayoutCharacter::OnGeometryCalculated()
	{
		if (m_Face != nullptr && m_Metrics != nullptr)
		{
			glm::vec2 position(
				m_Position.x,
				m_Position.y - m_Face->GetAscent() + m_Face->GetDescent()
			);
			glm::vec2 dimensions(
				m_Metrics->advance + m_Adjustment.x,
				m_Face->GetLineHeight()
			);

			m_LayoutGeometry.SetTopLeft(position);
			m_LayoutGeometry.SetDimensions(dimensions);
		}
		else
		{
			m_LayoutGeometry = BoundingBox(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
		}

		if (m_Metrics != nullptr)
		{
			m_ElementGeometry = m_Metrics->bounding_box.GetTranslated(m_Position);
		}
		else
		{
			m_ElementGeometry = BoundingBox(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
		}
	}

	void TextLayoutCharacter::OnAccepted(ITextLayoutVisitor& a_Visitor)
	{
	}

}; // namespace ExLibris