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
	
		MockFont(Family* a_Family)
			: IFont(a_Family)
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

		FontFace* CreateFace(const FaceOptions& a_Options)
		{
			FontFace* face = new FontFace(this);
			face->SetSize(a_Options.size);
			face->SetLineHeight(a_Options.size * 2.0f);

			return face;
		}
	
	}; // class MockFont

}; // namespace ExLibris

#endif