#ifndef _FONTFACEFREETYPE_H_
#define _FONTFACEFREETYPE_H_

namespace ExLibris
{
	class FontFreetype;
	struct Glyph;
}

namespace ExLibris
{

	class FontFaceFreetype
	{
	
	public:
	
		FontFaceFreetype(const FontFreetype* a_Font);
		~FontFaceFreetype();
	
		const FontFreetype* GetFont() const;

		float GetSize() const;
		float GetLineHeight() const;

		Glyph* FindGlyph(unsigned int a_CodepointUtf32) const;

		bool LoadGlyphs(FT_Face a_FontData, float a_Size);

	private:

		const FontFreetype* m_Font;
		float m_Size;
		float m_LineHeight;
		std::map<unsigned int, Glyph*> m_Glyphs;
	
	}; // class FontFaceFreetype

}; // namespace ExLibris

#endif