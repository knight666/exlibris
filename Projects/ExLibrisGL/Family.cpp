#include "ExLibrisGL.PCH.h"

#include "Family.h"

namespace ExLibris
{

	Family::Family(Library* a_Library, const std::string& a_Name)
		: m_Library(a_Library)
		, m_Name(a_Name)
	{
	}
	
	Family::~Family()
	{
	}

	Library* Family::GetLibrary() const
	{
		return m_Library;
	}

	const std::string& Family::GetName() const
	{
		return m_Name;
	}

}; // namespace ExLibris