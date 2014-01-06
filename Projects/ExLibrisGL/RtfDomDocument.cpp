#include "ExLibrisGL.PCH.h"

#include "RtfDomDocument.h"

#include "RtfDomElement.h"

namespace ExLibris
{

	RtfDomDocument::RtfDomDocument()
		: m_RootElement(new RtfDomElement())
	{
	}

	RtfDomDocument::~RtfDomDocument()
	{
		for (std::map<int, RtfFont*>::iterator font_it = m_FontTable.begin(); font_it != m_FontTable.end(); ++font_it)
		{
			delete font_it->second;
		}
		m_FontTable.clear();

		delete m_RootElement;
	}

	RtfTextFormat& RtfDomDocument::GetTextFormat()
	{
		return m_TextFormat;
	}

	RtfFont& RtfDomDocument::GetFont(int a_Index)
	{
		RtfFont* font = nullptr;

		std::map<int, RtfFont*>::iterator found = m_FontTable.find(a_Index);
		if (found != m_FontTable.end())
		{
			font = found->second;
		}
		else
		{
			font = new RtfFont;

			m_FontTable.insert(std::make_pair(a_Index, font));
		}

		return *font;
	}

	void RtfDomDocument::AddColor(const RtfColor& a_Color)
	{
		m_ColorTable.push_back(a_Color);
	}

	bool RtfDomDocument::TryGetColor(RtfColor& a_Target, int a_Index) const
	{
		if (a_Index < 0 || a_Index > (int)m_ColorTable.size())
		{
			return false;
		}

		a_Target = m_ColorTable[a_Index];

		return true;
	}

	RtfDomElement* RtfDomDocument::GetRootElement() const
	{
		return m_RootElement;
	}

}; // namespace ExLibris