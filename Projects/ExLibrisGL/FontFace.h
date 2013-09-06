#ifndef _FONTFACE_H_
#define _FONTFACE_H_

namespace ExLibris
{
	class FontFreetype;
	struct Glyph;
}

namespace ExLibris
{

	class FontFace
	{
	
	public:
	
		FontFace(const FontFreetype* a_Font);
		~FontFace();
	
		const FontFreetype* GetFont() const;

		float GetSize() const;
		void SetSize(float a_Size);

		float GetLineHeight() const;
		void SetLineHeight(float a_LineHeight);

		void AddGlyph(Glyph* a_Glyph);
		Glyph* FindGlyph(unsigned int a_CodepointUtf32) const;

	private:

		const FontFreetype* m_Font;
		float m_Size;
		float m_LineHeight;
		std::map<unsigned int, Glyph*> m_Glyphs;
	
	}; // class FontFace

}; // namespace ExLibris

#endif