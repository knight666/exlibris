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

		m_Error = FT_Select_Charmap(font_loaded, FT_ENCODING_UNICODE);
		if (m_Error != 0 || font_loaded->charmap == nullptr)
		{
			return nullptr;
		}

		Family* family = m_Library->CreateFamily(font_loaded->family_name);

		FontFreetype* font = new FontFreetype(family);

		font->SetFontData(font_loaded, font_file_data, (size_t)font_file_size);

		font->SetWeight(((font_loaded->style_flags & FT_STYLE_FLAG_BOLD) != 0) ? eWeight_Bold : eWeight_Normal);
		font->SetStyle(((font_loaded->style_flags & FT_STYLE_FLAG_ITALIC) != 0) ? eStyle_Italicized : eStyle_None);

		return font;
	}

}; // namespace ExLibris