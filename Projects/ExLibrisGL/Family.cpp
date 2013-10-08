#include "ExLibrisGL.PCH.h"

#include "Family.h"

namespace ExLibris
{

	Family::Family(Library* a_Library, const std::string& a_Name)
		: m_Library(a_Library)
		, m_Name(a_Name)
	{
	}
	
	Family::~Family()
	{
		for (std::set<IFont*>::iterator font_it = m_Fonts.begin(); font_it != m_Fonts.end(); ++font_it)
		{
			delete *font_it;
		}
	}

	Library* Family::GetLibrary() const
	{
		return m_Library;
	}

	const std::string& Family::GetName() const
	{
		return m_Name;
	}

	size_t Family::GetFontCount() const
	{
		return m_Fonts.size();
	}

	void Family::AddFont(IFont* a_Font)
	{
		std::set<IFont*>::iterator found = m_Fonts.find(a_Font);
		if (found == m_Fonts.end())
		{
			m_Fonts.insert(a_Font);
		}
	}

	IFont* Family::FindFont(Weight a_Weight, Style a_Style) const
	{
		IFont* match = nullptr;

		for (std::set<IFont*>::const_iterator font_it = m_Fonts.begin(); font_it != m_Fonts.end(); ++font_it)
		{
			IFont* font = *font_it;

			int score = 0;

			if (font->GetStyle() == a_Style)
			{
				match = font;
				score++;
			}

			if (font->GetWeight() == a_Weight)
			{
				match = font;
				score++;
			}

			if (score == 2)
			{
				break;
			}
		}

		return match;
	}

}; // namespace ExLibris