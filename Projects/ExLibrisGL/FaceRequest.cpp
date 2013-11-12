#include "ExLibrisGL.PCH.h"

#include "FaceRequest.h"

namespace ExLibris
{

	FaceRequest::FaceRequest()
		: m_FamilyName("")
		, m_HasFamilyName(false)
		, m_Size(0.0f)
		, m_HasSize(false)
	{
	}

	bool FaceRequest::HasFamilyName() const
	{
		return m_HasFamilyName;
	}

	const std::string& FaceRequest::GetFamilyName() const
	{
		return m_FamilyName;
	}

	void FaceRequest::SetFamilyName(const std::string& a_Name)
	{
		m_FamilyName = a_Name;
		m_HasFamilyName = true;
	}

	bool FaceRequest::HasSize() const
	{
		return m_HasSize;
	}

	float FaceRequest::GetSize() const
	{
		return m_Size;
	}

	void FaceRequest::SetSize(float a_Size)
	{
		m_Size = a_Size;
		m_HasSize = true;
	}

}; // namespace ExLibris