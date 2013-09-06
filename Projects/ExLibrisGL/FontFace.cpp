#include "ExLibrisGL.PCH.h"

#include "FontFace.h"

#include "FontFreetype.h"
#include "Glyph.h"

namespace ExLibris
{

	FontFace::FontFace(const FontFreetype* a_Font)
		: m_Font(a_Font)
		, m_Size(0.0f)
		, m_LineHeight(0.0f)
	{
	}
	
	FontFace::~FontFace()
	{
		for (std::map<unsigned int, Glyph*>::iterator glyph_it = m_Glyphs.begin(); glyph_it != m_Glyphs.end(); ++glyph_it)
		{
			delete glyph_it->second;
		}
		m_Glyphs.clear();
	}

	const FontFreetype* FontFace::GetFont() const
	{
		return m_Font;
	}

	float FontFace::GetSize() const
	{
		return m_Size;
	}

	void FontFace::SetSize(float a_Size)
	{
		m_Size = a_Size;
	}

	float FontFace::GetLineHeight() const
	{
		return m_LineHeight;
	}

	void FontFace::SetLineHeight(float a_LineHeight)
	{
		m_LineHeight = a_LineHeight;
	}

	void FontFace::AddGlyph(Glyph* a_Glyph)
	{
		m_Glyphs.insert(std::make_pair(a_Glyph->index, a_Glyph));
	}

	Glyph* FontFace::FindGlyph(unsigned int a_CodepointUtf32) const
	{
		if (m_Font == nullptr)
		{
			return nullptr;
		}

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