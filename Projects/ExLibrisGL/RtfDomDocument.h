#pragma once

#include "RtfColor.h"
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

		void AddColor(const RtfColor& a_Color);
		bool TryGetColor(RtfColor& a_Target, int a_Index) const;

		RtfDomElement* GetRootElement() const;

	private:

		RtfTextFormat m_TextFormat;

		std::map<int, RtfFont*> m_FontTable;
		std::vector<RtfColor> m_ColorTable;

		RtfDomElement* m_RootElement;

	}; // class RtfDomDocument

}; // namespace ExLibris