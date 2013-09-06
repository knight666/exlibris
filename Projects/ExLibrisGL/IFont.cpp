#include "ExLibrisGL.PCH.h"

#include "IFont.h"

namespace ExLibris
{

	IFont::IFont(const std::string& a_FamilyName)
		: m_FamilyName(a_FamilyName)
	{
	}
	
	IFont::~IFont()
	{
	}

	const std::string& IFont::GetFamilyName() const
	{
		return m_FamilyName;
	}

}; // namespace ExLibris