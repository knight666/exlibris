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

#include "FaceRequest.h"
#include "ITextLayoutElement.h"

namespace ExLibris
{
	class Face;
	class Library;
	class TextLayoutCharacter;
	class TextLayoutLine;
	class TextLayoutSection;
	class TextParserMarkdown;
}

namespace ExLibris
{

	class TextLayoutDocument
		: public ITextLayoutElement
	{

	private:

		struct Section;
		struct CharacterCollection;

		struct Character
		{
			int codepoint;
			Section* section;
			CharacterCollection* collection;
		};

		struct Section
		{
			Face* face;
			std::vector<int> codepoints;
		};

		struct CharacterCollection
		{
			enum Type
			{
				eType_Whitespace,
				eType_Characters,
				eType_Newline,
				eType_End
			};

			Type type;
			std::vector<int> codepoints;
			std::vector<Section*> sections;
			Character* character_first;
			Character* character_last;
		};
	
	public:
	
		TextLayoutDocument(Library* a_Library);
		~TextLayoutDocument();

		const std::vector<TextLayoutLine*>& GetLines() const;

		void SetDefaultFamily(const std::string& a_FamilyName);
		void SetDefaultSize(float a_Size);
	
		void SetParser(TextParserMarkdown* a_Parser);

		void SetText(const std::string& a_Text);

		void Layout();

	private:

		void OnGeometryCalculated();
		void OnAccepted(ITextLayoutVisitor& a_Visitor);

	private:

		void _ParseTextToCollections();
		void _LayoutGlyphs();

		void _ChangeFace();

		void _AddCharacter(int a_Codepoint);
		void _ClearCharacters();

		void _AddSection();
		void _ClearSections();

		void _AddCollection(CharacterCollection::Type a_Type);
		void _ClearCollections();

	private:

		Library* m_Library;
		TextParserMarkdown* m_Parser;
		std::string m_Text;

		FaceRequest m_Request;
		Face* m_FaceCurrent;
		bool m_FaceDirty;

		std::vector<Character*> m_Characters;

		std::vector<TextLayoutLine*> m_Lines;
		TextLayoutLine* m_LineCurrent;

		std::vector<CharacterCollection*> m_Collections;
		CharacterCollection* m_CollectionCurrent;

		std::vector<Section*> m_Sections;
		Section* m_SectionCurrent;

		TextLayoutCharacter* m_CharacterCurrent;
		glm::vec2 m_Cursor;
	
	}; // class TextLayoutDocument

}; // namespace ExLibris