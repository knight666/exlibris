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

#include "Library.h"
#include "TextLayoutSection.h"
#include "TextParserMarkdown.h"

namespace ExLibris
{

	TextLayoutDocument::TextLayoutDocument(Library* a_Library)
		: m_Library(a_Library)
		, m_Parser(nullptr)
		, m_FaceCurrent(nullptr)
		, m_FaceDirty(true)
		, m_SectionCurrent(nullptr)
	{
		if (m_Library != nullptr)
		{
			m_FaceCurrent = m_Library->RequestFace(m_Request);
		}
	}
	
	TextLayoutDocument::~TextLayoutDocument()
	{
		_ClearCollections();
		_ClearSections();
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
			_ParseTextToGlyphs();
		}

		int i = 0;
	}

	void TextLayoutDocument::OnGeometryCalculated()
	{
	}

	void TextLayoutDocument::OnAccepted(ITextLayoutVisitor& a_Visitor)
	{
	}

	void TextLayoutDocument::_ParseTextToGlyphs()
	{
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

				m_CollectionCurrent->codepoints.push_back(token.codepoint);
				m_SectionCurrent->codepoints.push_back(token.codepoint);
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
			else
			{
				m_SectionCurrent->face = m_FaceCurrent;
			}
		}
	}

	void TextLayoutDocument::_AddSection()
	{
		m_SectionCurrent = new Section;
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