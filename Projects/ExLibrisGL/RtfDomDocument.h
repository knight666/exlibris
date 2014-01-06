#pragma once

#include "RtfColorTable.h"
#include "RtfFont.h"
#include "RtfTextFormat.h"

namespace ExLibris
{
	class RtfDomElement;
};

namespace ExLibris
{

	class RtfDomDocument
	{

	public:

		RtfDomDocument();
		~RtfDomDocument();

		RtfTextFormat& GetTextFormat();

		RtfFont& GetFont(int a_Index);

		RtfColorTable* GetColorTable() const;

		RtfDomElement* GetRootElement() const;

	private:

		RtfTextFormat m_TextFormat;

		std::map<int, RtfFont*> m_FontTable;
		RtfColorTable* m_ColorTable;

		RtfDomElement* m_RootElement;

	}; // class RtfDomDocument

}; // namespace ExLibris