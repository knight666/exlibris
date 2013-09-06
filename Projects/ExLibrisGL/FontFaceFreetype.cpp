#include "ExLibrisGL.PCH.h"

#include "FontFaceFreetype.h"

#include "FontFreetype.h"
#include "Glyph.h"

namespace ExLibris
{

	FontFaceFreetype::FontFaceFreetype(const FontFreetype* a_Font)
		: m_Font(a_Font)
		, m_Size(0.0f)
		, m_LineHeight(0.0f)
	{
	}
	
	FontFaceFreetype::~FontFaceFreetype()
	{
		for (std::map<unsigned int, Glyph*>::iterator glyph_it = m_Glyphs.begin(); glyph_it != m_Glyphs.end(); ++glyph_it)
		{
			delete glyph_it->second;
		}
		m_Glyphs.clear();
	}

	const FontFreetype* FontFaceFreetype::GetFont() const
	{
		return m_Font;
	}

	float FontFaceFreetype::GetSize() const
	{
		return m_Size;
	}

	void FontFaceFreetype::SetSize(float a_Size)
	{
		m_Size = a_Size;
	}

	float FontFaceFreetype::GetLineHeight() const
	{
		return m_LineHeight;
	}

	void FontFaceFreetype::SetLineHeight(float a_LineHeight)
	{
		m_LineHeight = a_LineHeight;
	}

	void FontFaceFreetype::AddGlyph(Glyph* a_Glyph)
	{
		m_Glyphs.insert(std::make_pair(a_Glyph->index, a_Glyph));
	}

	Glyph* FontFaceFreetype::FindGlyph(unsigned int a_CodepointUtf32) const
	{
		unsigned int codepoint = (unsigned int)m_Font->GetIndexFromCodepoint(a_CodepointUtf32);
		if (codepoint == 0)
		{
			return nullptr;
		}

		std::map<unsigned int, Glyph*>::const_iterator found = m_Glyphs.find(codepoint);
		if (found != m_Glyphs.end())
		{
			return found->second;
		}

		return nullptr;
	}

}; // namespace ExLibris