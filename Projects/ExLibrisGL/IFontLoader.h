#ifndef _IFONTLOADER_H_
#define _IFONTLOADER_H_

namespace ExLibris
{
	class IFont;
	class Library;
}

namespace ExLibris
{

	class IFontLoader
	{
	
	public:
	
		IFontLoader(Library* a_Library)
			: m_Library(a_Library)
		{
		}

		virtual ~IFontLoader()
		{
		}
	
		virtual IFont* LoadFont(const std::string& a_Path) = 0;

	protected:

		Library* m_Library;
	
	}; // class IFontLoader

}; // namespace ExLibris

#endif