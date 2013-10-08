#ifndef _IFONT_H_
#define _IFONT_H_

#include "Style.h"
#include "Weight.h"

namespace ExLibris
{
	class FontFace;
}

namespace ExLibris
{

	class IFont
	{
	
	public:
	
		IFont(const std::string& a_FamilyName);
		virtual ~IFont();
	
		const std::string& GetFamilyName() const;

		Weight GetWeight() const;
		Style GetStyle() const;

		virtual unsigned int GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const = 0;

		virtual FontFace* CreateFace(float a_Size) const = 0;

	protected:

		std::string m_FamilyName;
		Weight m_Weight;
		Style m_Style;
	
	}; // class IFont

}; // namespace ExLibris

#endif