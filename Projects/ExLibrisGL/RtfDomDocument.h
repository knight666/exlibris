#pragma once

#include "RtfFont.h"
#include "RtfTextFormat.h"

namespace ExLibris
{

	class RtfDomDocument
	{

	public:

		RtfDomDocument();
		~RtfDomDocument();

		RtfTextFormat& GetTextFormat();

		RtfFont& GetFont(int a_Index);

	private:

		RtfTextFormat m_TextFormat;

		std::map<int, RtfFont*> m_FontTable;

	}; // class RtfDomDocument

}; // namespace ExLibris