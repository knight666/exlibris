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
		, m_FaceCurrent(nullptr)
		, m_FaceDirty(true)
		, m_CharacterCurrent(nullptr)
		, m_LineCurrent(nullptr)
		, m_SectionCurrent(nullptr)
		, m_CollectionCurrent(nullptr)
	{
		if (m_Library != nullptr)
		{
			m_FaceCurrent = m_Library->RequestFace(m_Request);
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

	void TextLayoutDocument::SetDefaultFamily(const std::string& a_FamilyName)
	{
		m_Request.SetFamilyName(a_FamilyName);

		m_FaceDirty = true;
	}

	void TextLayoutDocument::SetDefaultSize(float a_Size)
	{
		m_Request.SetSize(a_Size);

		m_FaceDirty = true;
	}

	void TextLayoutDocument::SetParser(TextParserMarkdown* a_Parser)
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

		for (std::vector<TextLayoutLine*>::iterator line_it = m_Lines.begin(); line_it != m_Lines.end(); ++line_it)
		{
			TextLayoutLine* line = *line_it;

			line->CalculateGeometry();
		}
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

		_ChangeFace();
		m_FaceDirty = false;

		while (m_Parser->ReadToken())
		{
			const TextToken& token = m_Parser->GetToken();

			if (token.changes == TextToken::eChanged_None)
			{
				CharacterCollection::Type collection_type_found = CharacterCollection::eType_End;

				switch (token.codepoint)
				{

				case '\n':
					{
						collection_type_found = CharacterCollection::eType_Newline;

					} break;

				case ' ':
					{
						collection_type_found = CharacterCollection::eType_Whitespace;

					} break;

				default:
					{
						collection_type_found = CharacterCollection::eType_Characters;

					} break;
				}

				if (m_CollectionCurrent == nullptr || collection_type_found != m_CollectionCurrent->type)
				{
					_AddCollection(collection_type_found);
				}

				_AddCharacter(token.codepoint);
			}
			else
			{
				if (token.changes & TextToken::eChanged_FamilyName)
				{
					m_Request.SetFamilyName(token.family_name);
					m_FaceDirty = true;
				}

				if (token.changes & TextToken::eChanged_Size)
				{
					m_Request.SetSize(token.size);
					m_FaceDirty = true;
				}

				if (token.changes & TextToken::eChanged_Weight)
				{
					m_Request.SetWeight(token.weight);
					m_FaceDirty = true;
				}

				if (token.changes & TextToken::eChanged_Style)
				{
					m_Request.SetStyle(token.style);
					m_FaceDirty = true;
				}

				if (m_FaceDirty)
				{
					_ChangeFace();

					m_FaceDirty = false;
				}
			}
		}
	}

	void TextLayoutDocument::_LayoutGlyphs()
	{
		m_Cursor = glm::vec2(0.0f, 0.0f);

		CharacterCollection* collection_current = nullptr;
		Section* section_current = nullptr;

		m_CharacterCurrent = nullptr;

		m_LineCurrent = new TextLayoutLine;
		m_Lines.push_back(m_LineCurrent);

		for (std::vector<Character*>::iterator character_it = m_Characters.begin(); character_it != m_Characters.end(); ++character_it)
		{
			Character* character = *character_it;

			int codepoint = character->codepoint;
			Face* face = character->section->face;

			// reset kerning when face changes

			if (section_current != character->section)
			{
				m_CharacterCurrent = nullptr;
				section_current = character->section;
			}

			TextLayoutCharacter* character_next = new TextLayoutCharacter(face, codepoint);
			character_next->SetPosition(m_Cursor);
			m_LineCurrent->AddChild(character_next);

			m_Cursor.x += character_next->GetMetrics()->advance;

			if (m_CharacterCurrent != nullptr)
			{
				glm::vec2 adjustment;
				if (face->TryGetKerningAdjustment(adjustment, m_CharacterCurrent->GetCodepoint(), codepoint))
				{
					character_next->SetKerningAdjustment(adjustment);
				}
			}

			m_CharacterCurrent = character_next;
		}
	}

	void TextLayoutDocument::_ChangeFace()
	{
		Face* found = m_Library->RequestFace(m_Request);
		if (found != m_FaceCurrent)
		{
			m_FaceCurrent = found;

			// if the section does not have any codepoints,
			// we can just set its face

			if (m_SectionCurrent->codepoints.size() > 0)
			{
				_AddSection();

				// reset the collection so that future codepoints
				// are added to the next collection

				m_CollectionCurrent = nullptr;
			}
			
			m_SectionCurrent->face = m_FaceCurrent;
		}
	}

	void TextLayoutDocument::_AddCharacter(int a_Codepoint)
	{
		Character* character = new Character;
		character->codepoint = a_Codepoint;
		character->collection = m_CollectionCurrent;
		character->section = m_SectionCurrent;
		m_Characters.push_back(character);

		m_CollectionCurrent->codepoints.push_back(a_Codepoint);
		m_SectionCurrent->codepoints.push_back(a_Codepoint);
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