#include "ExLibrisGL.PCH.h"

#include "RtfDomDocument.h"

namespace ExLibris
{

	RtfDomDocument::RtfDomDocument()
	{
	}

	RtfDomDocument::~RtfDomDocument()
	{
		for (std::map<int, RtfFont*>::iterator font_it = m_FontTable.begin(); font_it != m_FontTable.end(); ++font_it)
		{
			delete font_it->second;
		}
		m_FontTable.clear();
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

}; // namespace ExLibris