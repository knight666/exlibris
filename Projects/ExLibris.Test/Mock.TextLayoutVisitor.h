#ifndef _MOCK_TEXTLAYOUTVISITOR_H_
#define _MOCK_TEXTLAYOUTVISITOR_H_

#include <ITextLayoutVisitor.h>

namespace ExLibris
{

	class MockTextLayoutVisitor
		: public ITextLayoutVisitor
	{
	
	public:
	
		MockTextLayoutVisitor()
			: face(nullptr)
			, line_current(nullptr)
		{
		}

		~MockTextLayoutVisitor()
		{
			for (std::vector<VisitLine*>::iterator line_it = lines.begin(); line_it != lines.end(); ++line_it)
			{
				delete *line_it;
			}

			for (std::vector<VisitGlyph*>::iterator glyph_it = glyphs.begin(); glyph_it != glyphs.end(); ++glyph_it)
			{
				delete *glyph_it;
			}
		}
	
		void VisitTextBegin(const Face* a_Face, const BoundingBox& a_BoundingBox)
		{
			face = a_Face;
			dimensions = a_BoundingBox.GetDimensions();
		}

		void VisitTextLineBegin(const TextLine* a_Line)
		{
			line_current = new VisitLine;
			line_current->glyph_count = a_Line->characters.size();
			line_current->offset = a_Line->position;
			line_current->width = a_Line->dimensions.x;

			lines.push_back(line_current);
		}

		void VisitTextCharacter(const TextCharacter* a_Character)
		{
			VisitGlyph* glyph = new VisitGlyph;
			glyph->metrics = a_Character->metrics;
			glyph->x = a_Character->x;
			glyph->advance = a_Character->advance;

			line_current->text.push_back((wchar_t)a_Character->identifier);

			glyphs.push_back(glyph);
		}

		void VisitTextWhitespace(const TextCharacter* a_Character)
		{
			VisitGlyph* glyph = new VisitGlyph;
			glyph->metrics = a_Character->metrics;
			glyph->x = a_Character->x;
			glyph->advance = a_Character->advance;

			line_current->text.push_back((wchar_t)a_Character->identifier);

			glyphs.push_back(glyph);
		}

		void VisitTextLineEnd()
		{
		}

		void VisitTextEnd()
		{
		}

	public:

		const Face* face;
		glm::vec2 dimensions;

		struct VisitLine
		{
			size_t glyph_count;
			glm::vec2 offset;
			float width;
			std::wstring text;
		};
		std::vector<VisitLine*> lines;

		VisitLine* line_current;

		struct VisitGlyph
		{
			const GlyphMetrics* metrics;
			float x;
			float advance;
		};
		std::vector<VisitGlyph*> glyphs;
	
	}; // class MockTextLayoutVisitor

}; // namespace ExLibris

#endif