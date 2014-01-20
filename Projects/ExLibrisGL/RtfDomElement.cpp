#include "ExLibrisGL.PCH.h"

#include "RtfDomElement.h"

#include "RtfDomDocument.h"

namespace ExLibris
{

	struct RtfDomElement::ParseState
	{
		ParseState()
			: character_encoding_found(false)
		{
		}

		bool character_encoding_found;
	};

	RtfDomElement::RtfDomElement(RtfDomDocument& a_Document)
		: m_Document(a_Document)
		, m_TextFormat(new RtfTextFormat(m_Document, &m_Document.GetTextFormat()))
		, m_Parent(nullptr)
		, m_SiblingPrevious(nullptr)
		, m_SiblingNext(nullptr)
		, m_ChildPrevious(nullptr)
	{
	}

	RtfDomElement::~RtfDomElement()
	{
		delete m_TextFormat;

		for (std::vector<RtfDomElement*>::iterator child_it = m_Children.begin(); child_it != m_Children.end(); ++child_it)
		{
			delete *child_it;
		}
		m_Children.clear();
	}

	RtfTextFormat& RtfDomElement::GetTextFormat() const
	{
		return *m_TextFormat;
	}

	RtfDomElement* RtfDomElement::GetParent() const
	{
		return m_Parent;
	}

	RtfDomElement* RtfDomElement::GetSiblingPrevious() const
	{
		return m_SiblingPrevious;
	}

	RtfDomElement* RtfDomElement::GetSiblingNext() const
	{
		return m_SiblingNext;
	}

	size_t RtfDomElement::GetChildrenCount() const
	{
		return m_Children.size();
	}

	RtfDomElement* RtfDomElement::GetChild(size_t a_Index) const
	{
		if (a_Index >= m_Children.size())
		{
			return nullptr;
		}
		else
		{
			return m_Children[a_Index];
		}
	}

	RtfDomElement* RtfDomElement::AddChild()
	{
		RtfDomElement* child = new RtfDomElement(m_Document);
		child->m_Parent = this;
		child->m_TextFormat = new RtfTextFormat(*m_TextFormat);
		child->m_SiblingPrevious = m_ChildPrevious;

		if (m_ChildPrevious != nullptr)
		{
			m_ChildPrevious->m_SiblingNext = child;
		}

		m_ChildPrevious = child;

		m_Children.push_back(child);

		return child;
	}

	IRtfParseable::Result RtfDomElement::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		IRtfParseable::Result result = IRtfParseable::eResult_Propagate;

		result = m_TextFormat->Parse(a_State, a_Token);
		if (result != IRtfParseable::eResult_Propagate)
		{
			return result;
		}

		return result;
	}

	IRtfParseable::Result RtfDomElement::_ParseText(RtfParserState& a_State, const RtfToken& a_Token)
	{
		InnerText += a_Token.value;

		return eResult_Handled;
	}

}; // namespace ExLibris