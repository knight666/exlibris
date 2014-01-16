#pragma once

#include "RtfTextFormat.h"

namespace ExLibris
{
	class RtfDomDocument;
}

namespace ExLibris
{

	class RtfDomElement
	{

		friend class RtfDomElement;

	public:

		RtfDomElement(RtfDomDocument& a_Document);
		~RtfDomElement();

		std::string InnerText;

		RtfTextFormat& GetTextFormat() const;

		RtfDomElement* GetParent() const;

		RtfDomElement* GetSiblingPrevious() const;
		RtfDomElement* GetSiblingNext() const;

		size_t GetChildrenCount() const;
		RtfDomElement* GetChild(size_t a_Index) const;
		RtfDomElement* AddChild();

	private:

		RtfDomDocument& m_Document;
		RtfTextFormat* m_TextFormat;
		RtfDomElement* m_Parent;
		RtfDomElement* m_SiblingPrevious;
		RtfDomElement* m_SiblingNext;
		std::vector<RtfDomElement*> m_Children;
		RtfDomElement* m_ChildPrevious;

	}; // class RtfDomElement

}; // namespace ExLibris