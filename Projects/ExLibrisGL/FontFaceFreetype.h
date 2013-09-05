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
	
		FontFaceFreetype(FontFreetype* a_Font);
		~FontFaceFreetype();
	
		FontFreetype* GetFont() const;

		float GetSize() const;

		bool LoadGlyphs(FT_Face a_FontData, float a_Size);

	private:

		FontFreetype* m_Font;
		float m_Size;
	
	}; // class FontFaceFreetype

}; // namespace ExLibris

#endif