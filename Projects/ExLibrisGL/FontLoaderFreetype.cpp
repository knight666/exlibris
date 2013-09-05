#include "ExLibrisGL.PCH.h"

#include "FontLoaderFreetype.h"

namespace ExLibris
{

	FontLoaderFreetype::FontLoaderFreetype()
		: m_Error(0)
	{
		m_Error = FT_Init_FreeType(&m_Library);
	}

	FontLoaderFreetype::~FontLoaderFreetype()
	{
		FT_Done_FreeType(m_Library);
	}

	FT_Library FontLoaderFreetype::GetLibrary() const
	{
		return m_Library;
	}

	FT_Face FontLoaderFreetype::LoadFontFace(const std::string& a_Path)
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

		FT_Face result;
		m_Error = FT_New_Memory_Face(m_Library, font_file_data, (FT_Long)font_file_size, 0, &result);

		if (result->charmap == nullptr)
		{
			FT_Select_Charmap(result, FT_ENCODING_UNICODE);
			if (result->charmap == nullptr)
			{
				FT_Done_Face(result);
				return nullptr;
			}
		}

		return result;
	}

}; // namespace ExLibris