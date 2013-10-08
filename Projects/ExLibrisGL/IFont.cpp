#include "ExLibrisGL.PCH.h"

#include "IFont.h"

namespace ExLibris
{

	IFont::IFont(Family* a_Family)
		: m_Family(a_Family)
		, m_Weight(eWeight_Normal)
		, m_Style(eStyle_None)
	{
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

	Style IFont::GetStyle() const
	{
		return m_Style;
	}

}; // namespace ExLibris