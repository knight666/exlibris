#ifndef _FONTLOADERFREETYPE_H_
#define _FONTLOADERFREETYPE_H_

namespace ExLibris
{

	class FontLoaderFreetype
	{
	
	public:
	
		FontLoaderFreetype();
		~FontLoaderFreetype();

		FT_Library GetLibrary() const;

		FT_Face LoadFontFace(const std::string& a_Path);
	
	private:

		FT_Error m_Error;
		FT_Library m_Library;
	
	}; // class FontLoaderFreetype

}; // namespace ExLibris

#endif