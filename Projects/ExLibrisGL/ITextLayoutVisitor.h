#ifndef _ITEXTLAYOUTVISITOR_H_
#define _ITEXTLAYOUTVISITOR_H_

namespace ExLibris
{

	class ITextLayoutVisitor
	{

	public:

		virtual ~ITextLayoutVisitor()
		{
		}

		virtual void VisitTextBegin(const FontFace* a_Face, const glm::vec2& a_Dimensions) = 0;
		virtual void VisitTextLineBegin(size_t a_GlyphCount, const glm::vec2& a_Offset, float a_Width) = 0;
		virtual void VisitTextGlyph(const Glyph* a_Glyph, float a_X, float a_Advance) = 0;
		virtual void VisitTextLineEnd() = 0;
		virtual void VisitTextEnd() = 0;

	};

}; // namespace ExLibris

#endif