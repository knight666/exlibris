#ifndef _FONTFACEFREETYPE_H_
#define _FONTFACEFREETYPE_H_

namespace ExLibris
{

	class FontFaceFreetype
	{
	
	public:
	
		FontFaceFreetype(FT_Face a_FontFace);
		~FontFaceFreetype();
	
		bool LoadGlyphs(float a_Size);
	
	}; // class FontFaceFreetype

}; // namespace ExLibris

#endif