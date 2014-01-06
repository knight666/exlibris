#pragma once

#include "RtfTextFormat.h"

namespace ExLibris
{

	class RtfDomElement
	{

		friend class RtfDomElement;

	public:

		RtfDomElement();
		~RtfDomElement();

		RtfTextFormat TextFormat;

		std::string InnerText;

		RtfDomElement* GetParent() const;

		RtfDomElement* GetSiblingPrevious() const;
		RtfDomElement* GetSiblingNext() const;

		size_t GetChildrenCount() const;
		RtfDomElement* GetChild(size_t a_Index) const;
		RtfDomElement* AddChild();

	private:

		RtfDomElement* m_Parent;
		RtfDomElement* m_SiblingPrevious;
		RtfDomElement* m_SiblingNext;
		std::vector<RtfDomElement*> m_Children;
		RtfDomElement* m_ChildPrevious;

	}; // class RtfDomElement

}; // namespace ExLibris