#include "ExLibrisGL.PCH.h"

#include "FontLoaderFreetype.h"

#include "Library.h"

#include <memory>

namespace ExLibris
{

	FontLoaderFreetype::FontLoaderFreetype(Library* a_Library)
		: IFontLoader(a_Library)
		, m_Error(0)
	{
		FT_Init_FreeType(&m_FTLibrary);
	}

	FontLoaderFreetype::~FontLoaderFreetype()
	{
		for (std::vector<FT_Face>::iterator face_it = m_Faces.begin(); face_it != m_Faces.end(); ++face_it)
		{
			FT_Done_Face(*face_it);
		}
		m_Faces.clear();

		FT_Done_FreeType(m_FTLibrary);
	}

	FT_Library FontLoaderFreetype::GetLibrary() const
	{
		return m_FTLibrary;
	}

	IFont* FontLoaderFreetype::LoadFont(const std::string& a_Path)
	{
		std::fstream font_file(a_Path, std::fstream::in | std::fstream::binary);
		if (!font_file.is_open())
		{
			return nullptr;
		}

		font_file.seekg(0, std::ios_base::end);
		std::streamoff font_file_size = font_file.tellg();
		font_file.seekg(0, std::ios_base::beg);

		FT_Byte* font_file_data = new FT_Byte[(unsigned int)font_file_size];
		font_file.read((char*)font_file_data, font_file_size);

		font_file.close();

		FT_Face font_loaded;
		m_Error = FT_New_Memory_Face(m_FTLibrary, font_file_data, (FT_Long)font_file_size, 0, &font_loaded);
		if (m_Error != 0)
		{
			return nullptr;
		}

		m_Faces.push_back(font_loaded);

		Family* family = m_Library->CreateFamily(font_loaded->family_name);

		FontFreetype* font = new FontFreetype(family);
		if (!font->LoadFontData(font_loaded))
		{
			delete font;
			return nullptr;
		}

		return font;
	}

}; // namespace ExLibris