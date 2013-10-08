#include "ExLibrisGL.PCH.h"

#include "IFont.h"

namespace ExLibris
{

	IFont::IFont(const std::string& a_FamilyName)
		: m_FamilyName(a_FamilyName)
		, m_Weight(eWeight_Normal)
		, m_Style(eStyle_None)
	{
	}
	
	IFont::~IFont()
	{
	}

	const std::string& IFont::GetFamilyName() const
	{
		return m_FamilyName;
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