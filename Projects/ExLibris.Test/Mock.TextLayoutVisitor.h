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
	
		void VisitTextBegin(const FontFace* a_Face, const glm::vec2& a_Dimensions)
		{
			face = a_Face;
			dimensions = a_Dimensions;
		}

		void VisitTextLineBegin(size_t a_GlyphCount, const glm::vec2& a_Offset, float a_Width)
		{
			line_current = new VisitLine;
			line_current->glyph_count = a_GlyphCount;
			line_current->offset = a_Offset;
			line_current->width = a_Width;

			lines.push_back(line_current);
		}

		void VisitTextCharacter(const Glyph* a_Glyph, float a_X, float a_Advance)
		{
			VisitGlyph* glyph = new VisitGlyph;
			glyph->glyph = a_Glyph;
			glyph->x = a_X;
			glyph->advance = a_Advance;

			line_current->text.push_back((wchar_t)a_Glyph->index);

			glyphs.push_back(glyph);
		}

		void VisitTextWhitespace(float a_X, float a_Advance)
		{
			VisitGlyph* glyph = new VisitGlyph;
			glyph->glyph = nullptr;
			glyph->x = a_X;
			glyph->advance = a_Advance;

			glyphs.push_back(glyph);
		}

		void VisitTextLineEnd()
		{
		}

		void VisitTextEnd()
		{
		}

	public:

		const FontFace* face;
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
			const Glyph* glyph;
			float x;
			float advance;
		};
		std::vector<VisitGlyph*> glyphs;
	
	}; // class MockTextLayoutVisitor

}; // namespace ExLibris

#endif