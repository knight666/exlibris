#ifndef _IFONT_H_
#define _IFONT_H_

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

		virtual unsigned int GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const = 0;

		virtual FontFace* CreateFace(float a_Size) const = 0;

	protected:

		std::string m_FamilyName;
	
	}; // class IFont

}; // namespace ExLibris

#endif