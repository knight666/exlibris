#ifndef _FONTFACE_H_
#define _FONTFACE_H_

#include <map>

namespace ExLibris
{
	struct Glyph;
	class IFont;
}

namespace ExLibris
{

	class FontFace
	{
	
	public:
	
		FontFace(const IFont* a_Font);
		~FontFace();
	
		const IFont* GetFont() const;

		float GetSize() const;
		void SetSize(float a_Size);

		float GetLineHeight() const;
		void SetLineHeight(float a_LineHeight);

		void AddGlyph(Glyph* a_Glyph);
		Glyph* FindGlyph(unsigned int a_CodepointUtf32) const;

		bool TryGetKerning(Glyph* a_Left, Glyph* a_Right, glm::vec2& a_Target) const;

	private:

		const IFont* m_Font;
		float m_Size;
		float m_LineHeight;
		std::map<unsigned int, Glyph*> m_Glyphs;
	
	}; // class FontFace

}; // namespace ExLibris

#endif