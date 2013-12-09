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

#include "TextLayoutDocument.h"

#include "Face.h"
#include "GlyphMetrics.h"
#include "ITextParser.h"
#include "Library.h"
#include "TextLayoutCharacter.h"
#include "TextLayoutLine.h"
#include "TextLayoutSection.h"
#include "TextParserMarkdown.h"

namespace ExLibris
{

	TextLayoutDocument::TextLayoutDocument(Library* a_Library)
		: m_Library(a_Library)
		, m_Parser(nullptr)
		, m_CharacterCurrent(nullptr)
		, m_LineCurrent(nullptr)
		, m_SectionCurrent(nullptr)
		, m_CollectionCurrent(nullptr)
		, m_TextFormatCurrent(nullptr)
	{
		if (m_Library != nullptr)
		{
			m_TextFormatCurrent = new TextFormat(m_Library);

			TextFormat* default = new TextFormat(m_Library);
			m_TextFormats.push_back(default);
		}
	}
	
	TextLayoutDocument::~TextLayoutDocument()
	{
		_ClearCharacters();
		_ClearCollections();
		_ClearSections();
	}

	const std::vector<TextLayoutLine*>& TextLayoutDocument::GetLines() const
	{
		return m_Lines;
	}

	void TextLayoutDocument::SetDefaultTextFormat(const TextFormat& a_TextFormat)
	{
		TextFormat* default = *m_TextFormats.begin();
		*default = a_TextFormat;
	}

	void TextLayoutDocument::SetParser(ITextParser* a_Parser)
	{
		m_Parser = a_Parser;
	}

	void TextLayoutDocument::SetText(const std::string& a_Text)
	{
		m_Text = a_Text;
		if (m_Parser != nullptr)
		{
			m_Parser->SetInput(m_Text);
		}
	}

	void TextLayoutDocument::Layout()
	{
		if (m_Parser != nullptr)
		{
			_ParseTextToCollections();
		}

		_LayoutGlyphs();

		CalculateGeometry();
	}

	void TextLayoutDocument::OnGeometryCalculated()
	{
	}

	void TextLayoutDocument::OnAccepted(ITextLayoutVisitor& a_Visitor)
	{
	}

	void TextLayoutDocument::_ParseTextToCollections()
	{
		_ClearCharacters();

		_ClearCollections();
		m_CollectionCurrent = nullptr;

		_ClearSections();
		_AddSection();

		m_TextFormatCurrent = *m_TextFormats.begin();

		while (m_Parser->ReadToken())
		{
			const TextParserToken& token = m_Parser->GetToken();

			if (token.changes == TextParserToken::eChanged_None)
			{
				Character* character = new Character;
				character->codepoint = token.codepoint;
				character->type = token.type;
				character->format = m_TextFormatCurrent;
				
				m_Characters.push_back(character);
			}
			else
			{
				m_TextFormatCurrent = m_TextFormatCurrent->CreateSpecialization();

				if (token.changes & TextParserToken::eChanged_FamilyName)
				{
					m_TextFormatCurrent->SetFamilyName(token.family_name);
				}

				if (token.changes & TextParserToken::eChanged_Size)
				{
					m_TextFormatCurrent->SetSize(token.size);
				}

				if (token.changes & TextParserToken::eChanged_Weight)
				{
					m_TextFormatCurrent->SetWeight(token.weight);
				}

				if (token.changes & TextParserToken::eChanged_Style)
				{
					m_TextFormatCurrent->SetStyle(token.style);
				}

				if (token.changes & TextParserToken::eChanged_Color)
				{
					m_TextFormatCurrent->SetColor(token.color);
				}

				if (token.changes & TextParserToken::eChanged_BackgroundColor)
				{
					m_TextFormatCurrent->SetBackgroundColor(token.background_color);
				}

				m_TextFormats.push_back(m_TextFormatCurrent);
			}
		}
	}

	void TextLayoutDocument::_LayoutGlyphs()
	{
		m_Cursor = glm::vec2(0.0f, 0.0f);

		m_LineCurrent = new TextLayoutLine;
		m_Lines.push_back(m_LineCurrent);

		AddChild(m_LineCurrent);

		TextFormat* format_current = nullptr;
		Face* face_current = nullptr;

		for (std::vector<Character*>::iterator character_it = m_Characters.begin(); character_it != m_Characters.end(); ++character_it)
		{
			Character* character = *character_it;

			if (format_current != character->format)
			{
				// reset kerning if face changes

				if (face_current != character->format->GetFace())
				{
					m_CharacterCurrent = nullptr;
				}

				format_current = character->format;
				face_current = character->format->GetFace();
			}

			TextLayoutCharacter* character_next = new TextLayoutCharacter(character->format, character->codepoint);
			character_next->SetPosition(m_Cursor);

			m_Cursor.x += character_next->GetMetrics()->advance;

			if (m_CharacterCurrent != nullptr)
			{
				glm::vec2 adjustment;
				if (face_current->TryGetKerningAdjustment(adjustment, m_CharacterCurrent->GetCodepoint(), character->codepoint))
				{
					character_next->SetKerningAdjustment(adjustment);
				}
			}

			m_LineCurrent->AddChild(character_next);

			m_CharacterCurrent = character_next;
		}
	}

	void TextLayoutDocument::_ClearCharacters()
	{
		for (std::vector<Character*>::iterator character_it = m_Characters.begin(); character_it != m_Characters.end(); ++character_it)
		{
			delete *character_it;
		}
		m_Characters.clear();
	}

	void TextLayoutDocument::_AddSection()
	{
		m_SectionCurrent = new Section;
		m_SectionCurrent->face = nullptr;
		m_Sections.push_back(m_SectionCurrent);
	}

	void TextLayoutDocument::_ClearSections()
	{
		for (std::vector<Section*>::iterator section_it = m_Sections.begin(); section_it != m_Sections.end(); ++section_it)
		{
			delete *section_it;
		}
		m_Sections.clear();
	}

	void TextLayoutDocument::_AddCollection(CharacterCollection::Type a_Type)
	{
		m_CollectionCurrent = new CharacterCollection;
		m_CollectionCurrent->type = a_Type;
		m_CollectionCurrent->sections.push_back(m_SectionCurrent);
		m_Collections.push_back(m_CollectionCurrent);
	}

	void TextLayoutDocument::_ClearCollections()
	{
		for (std::vector<CharacterCollection*>::iterator collection_it = m_Collections.begin(); collection_it != m_Collections.end(); ++collection_it)
		{
			delete *collection_it;
		}
		m_Collections.clear();
	}

}; // namespace ExLibris