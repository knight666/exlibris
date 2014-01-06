#pragma once

#include "RtfCharacterSet.h"
#include "RtfFont.h"

namespace ExLibris
{

	class RtfDomDocument
	{

	public:

		RtfDomDocument();
		~RtfDomDocument();

		RtfCharacterSet GetCharacterSet() const;
		void SetCharacterSet(RtfCharacterSet a_CharacterSet);

		RtfFont& GetFont(int a_Index);

	private:

		RtfCharacterSet m_CharacterSet;
		std::map<int, RtfFont*> m_FontTable;

	}; // class RtfDomDocument

}; // namespace ExLibris