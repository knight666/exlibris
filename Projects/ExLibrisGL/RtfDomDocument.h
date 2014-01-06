#pragma once

#include "RtfFont.h"

namespace ExLibris
{

	class RtfDomDocument
	{

	public:

		RtfDomDocument();
		~RtfDomDocument();

		RtfFont& GetFont(int a_Index);

	private:

		std::map<int, RtfFont*> m_FontTable;

	}; // class RtfDomDocument

}; // namespace ExLibris