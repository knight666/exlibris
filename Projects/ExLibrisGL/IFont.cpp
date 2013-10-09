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
		for (std::map<float, FontFace*>::iterator face_it = m_Faces.begin(); face_it != m_Faces.end(); ++face_it)
		{
			delete face_it->second;
		}
		m_Faces.clear();
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

	FontFace* IFont::CreateFace(float a_Size)
	{
		std::map<float, FontFace*>::iterator found = m_Faces.find(a_Size);
		if (found != m_Faces.end())
		{
			return found->second;
		}
		else
		{
			FontFace* face = _CreateFace(a_Size);
			if (face != nullptr)
			{
				m_Faces.insert(std::make_pair(a_Size, face));
			}

			return face;
		}
	}

}; // namespace ExLibris