#ifndef _FONTFACEFREETYPE_H_
#define _FONTFACEFREETYPE_H_

namespace ExLibris
{
	class FontFreetype;
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

		bool LoadGlyphs(FT_Face a_FontData, float a_Size);

	private:

		const FontFreetype* m_Font;
		float m_Size;
		float m_LineHeight;
	
	}; // class FontFaceFreetype

}; // namespace ExLibris

#endif