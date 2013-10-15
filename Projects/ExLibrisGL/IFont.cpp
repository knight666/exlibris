#include "ExLibrisGL.PCH.h"

#include "IFont.h"

#include "Family.h"
#include "FontFace.h"

namespace ExLibris
{

	IFont::IFont(Family* a_Family)
		: m_Family(a_Family)
		, m_Weight(eWeight_Normal)
		, m_Style(eStyle_None)
	{
		if (m_Family != nullptr)
		{
			m_Family->AddFont(this);
		}
	}
	
	IFont::~IFont()
	{
	}

	Family* IFont::GetFamily() const
	{
		return m_Family;
	}

	Weight IFont::GetWeight() const
	{
		return m_Weight;
	}

	void IFont::SetWeight(Weight a_Weight)
	{
		m_Weight = a_Weight;
	}

	Style IFont::GetStyle() const
	{
		return m_Style;
	}

	void IFont::SetStyle(Style a_Style)
	{
		m_Style = a_Style;
	}

}; // namespace ExLibris