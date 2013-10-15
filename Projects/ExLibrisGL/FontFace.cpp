#include "ExLibrisGL.PCH.h"

#include "FontFace.h"

#include "Glyph.h"
#include "IFont.h"

namespace ExLibris
{

	FontFace::FontFace(const IFont* a_Font)
		: m_Font(a_Font)
		, m_Size(0.0f)
		, m_LineHeight(0.0f)
		, m_Ascender(0.0f)
		, m_Descender(0.0f)
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

	const IFont* FontFace::GetFont() const
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
	
	float FontFace::GetAscender() const
	{
		return m_Ascender;
	}

	void FontFace::SetAscender(float a_Ascender)
	{
		m_Ascender = a_Ascender;
	}

	float FontFace::GetDescender() const
	{
		return m_Descender;
	}

	void FontFace::SetDescender(float a_Descender)
	{
		m_Descender = a_Descender;
	}

	bool FontFace::AddGlyph(Glyph* a_Glyph)
	{
		std::map<unsigned int, Glyph*>::iterator found = m_Glyphs.find(a_Glyph->index);
		if (found != m_Glyphs.end())
		{
			return false;
		}

		m_Glyphs.insert(std::make_pair(a_Glyph->index, a_Glyph));

		return true;
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

	bool FontFace::TryGetKerning(Glyph* a_Left, Glyph* a_Right, glm::vec2& a_Target) const
	{
		if (a_Left == nullptr || a_Right == nullptr)
		{
			return false;
		}

		std::map<unsigned int, glm::vec2>::const_iterator found = a_Left->kernings.find(a_Right->index);
		if (found != a_Left->kernings.end())
		{
			a_Target = found->second;

			return true;
		}
		else
		{
			return false;
		}
	}

}; // namespace ExLibris