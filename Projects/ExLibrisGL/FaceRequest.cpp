#include "ExLibrisGL.PCH.h"

#include "FaceRequest.h"

namespace ExLibris
{

	FaceRequest::FaceRequest()
		: m_FamilyName("")
		, m_HasFamilyName(false)
		, m_Size(0.0f)
		, m_HasSize(false)
		, m_Weight(eWeight_Normal)
		, m_HasWeight(false)
		, m_Style(eStyle_None)
		, m_HasStyle(false)
	{
	}

	bool FaceRequest::HasFamilyName() const
	{
		return m_HasFamilyName;
	}

	const String& FaceRequest::GetFamilyName() const
	{
		return m_FamilyName;
	}

	void FaceRequest::SetFamilyName(const String& a_Name)
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

	bool FaceRequest::HasWeight() const
	{
		return m_HasWeight;
	}

	Weight FaceRequest::GetWeight() const
	{
		return m_Weight;
	}

	void FaceRequest::SetWeight(Weight a_Weight)
	{
		m_Weight = a_Weight;
		m_HasWeight = true;
	}

	bool FaceRequest::HasStyle() const
	{
		return m_HasStyle;
	}

	Style FaceRequest::GetStyle() const
	{
		return m_Style;
	}

	void FaceRequest::SetStyle(Style a_Style)
	{
		m_Style = a_Style;
		m_HasStyle = true;
	}

}; // namespace ExLibris