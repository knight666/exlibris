#ifndef _IFONT_H_
#define _IFONT_H_

#include "Style.h"
#include "Weight.h"

namespace ExLibris
{
	class Family;
	class FontFace;
}

namespace ExLibris
{

	class IFont
	{
	
	public:
	
		IFont(Family* a_Family);
		virtual ~IFont();
	
		Family* GetFamily() const;

		Weight GetWeight() const;
		void SetWeight(Weight a_Weight);

		Style GetStyle() const;
		void SetStyle(Style a_Style);

		virtual unsigned int GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const = 0;

		virtual FontFace* CreateFace(float a_Size) = 0;

	protected:

		Family* m_Family;
		Weight m_Weight;
		Style m_Style;
	
	}; // class IFont

}; // namespace ExLibris

#endif