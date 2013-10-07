#ifndef _TEXTLAYOUT_H_
#define _TEXTLAYOUT_H_

#include "Alignment.h"
#include "FontFace.h"
#include "ITextLayoutVisitor.h"
#include "SizePolicy.h"

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
	
		FontFace* GetFontFace() const;
		void SetFontFace(FontFace* a_Face);

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

		void Layout();

		void Accept(ITextLayoutVisitor& a_Visitor);

	private:

		enum GlyphType
		{
			eGlyphType_Character,
			eGlyphType_Whitespace,
			eGlyphType_NewLine,
			eGlyphType_End
		};

		struct TextGlyph
		{
			unsigned int identifier;
			Glyph* glyph;
			GlyphType type;
			float x;
			float advance;
		};

		struct TextLine
		{
			std::vector<TextGlyph*> glyphs;
			glm::vec2 position;
			float width;
		};

		std::vector<unsigned int> _AsciiToUtf32(const std::string& a_Text);
		void _ConvertTextToGlyphs();
		void _ConvertGlyphsToLines();
		void _WordWrappingNone(bool a_WidthFixed, bool a_HeightFixed);
		void _WordWrappingGreedy(bool a_WidthFixed, bool a_HeightFixed);
		void _CalculateDimensions();
		void _LayoutLines();

		bool _NextLine(bool a_HeightFixed);
		void _AddGlyphToCurrentLine(TextGlyph* a_Glyph);

	private:

		FontFace* m_Face;
		std::string m_Text;

		std::vector<TextGlyph*> m_Glyphs;
		bool m_GlyphsDirty;

		TextGlyph* m_GlyphSpace;

		std::vector<TextLine*> m_Lines;
		TextLine* m_LineCurrent;
		bool m_LinesDirty;

		glm::vec2 m_Cursor;
		glm::vec2 m_Dimensions;

		glm::vec2 m_SizeHint;
		HorizontalAlignment m_HorizontalAlignment;
		VerticalAlignment m_VerticalAlignment;
		WordWrapping m_WordWrapping;
		SizePolicy m_HorizontalPolicy;
		SizePolicy m_VerticalPolicy;
		bool m_LayoutDirty;
	
	}; // class TextLayout

}; // namespace ExLibris

#endif