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

#include "TextLayoutSection.h"

#include "Face.h"
#include "GlyphMetrics.h"
#include "TextLayoutCharacter.h"

namespace ExLibris
{

	TextLayoutSection::TextLayoutSection(Face* a_Face)
		: m_Face(a_Face)
		, m_Cursor(0.0f, 0.0f)
		, m_CharacterPrevious(nullptr)
	{
	}
	
	TextLayoutSection::~TextLayoutSection()
	{
	}

	TextLayoutCharacter* TextLayoutSection::AddCharacter(int a_Codepoint)
	{
		GlyphMetrics* metrics = m_Face->CreateMetrics(a_Codepoint);

		TextLayoutCharacter* character = new TextLayoutCharacter(m_Face, a_Codepoint, metrics);
		character->SetPosition(m_Cursor);

		if (m_CharacterPrevious != nullptr)
		{
			glm::vec2 adjustment;
			if (m_Face->TryGetKerningAdjustment(adjustment, m_CharacterPrevious->GetCodepoint(), a_Codepoint))
			{
				character->SetKerningAdjustment(adjustment);
			}
		}

		AddChild(character);
		m_Cursor.x += metrics->advance;

		m_CharacterPrevious = character;

		return character;
	}

	void TextLayoutSection::OnGeometryCalculated()
	{
	}

	void TextLayoutSection::OnAccepted(ITextLayoutVisitor& a_Visitor)
	{
	}

}; // namespace ExLibris