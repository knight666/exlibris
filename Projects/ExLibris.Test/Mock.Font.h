#ifndef _MOCK_FONT_H_
#define _MOCK_FONT_H_

#include <IFont.h>
#include <FontFace.h>

namespace ExLibris
{

	class MockFont
		: public IFont
	{
	
	public:
	
		MockFont(const std::string& a_FamilyName)
			: IFont(a_FamilyName)
		{
		}

		~MockFont()
		{
		}

		void SetWeight(Weight a_Weight)
		{
			m_Weight = a_Weight;
		}

		void SetStyle(Style a_Style)
		{
			m_Style = a_Style;
		}

		unsigned int GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const
		{
			return a_CodepointUtf32;
		}

		FontFace* CreateFace(float a_Size) const
		{
			FontFace* face = new FontFace(this);
			face->SetSize(a_Size);
			face->SetLineHeight(a_Size * 2.0f);

			return face;
		}
	
	}; // class MockFont

}; // namespace ExLibris

#endif