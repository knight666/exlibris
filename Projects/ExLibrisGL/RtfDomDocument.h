#pragma once

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

		RtfDomElement* GetRootElement() const;

	private:

		RtfTextFormat m_TextFormat;

		std::map<int, RtfFont*> m_FontTable;

		RtfDomElement* m_RootElement;

	}; // class RtfDomDocument

}; // namespace ExLibris