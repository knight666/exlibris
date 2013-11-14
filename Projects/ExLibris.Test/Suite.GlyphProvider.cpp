#include "ExLibris.Test.PCH.h"

#include <Face.h>
#include <GlyphProviderFreetype.h>
#include <Library.h>

using namespace ExLibris;

TEST(GlyphProvider, LoadFace)
{
	std::fstream in_stream("Fonts/Roboto/Roboto-BoldItalic.ttf", std::ios::in | std::ios::binary);

	in_stream.seekg(0, std::ios_base::end);
	std::streamoff font_file_size = in_stream.tellg();
	in_stream.seekg(0, std::ios_base::beg);

	ASSERT_GT(font_file_size, 0);

	FT_Byte* font_file_data = new FT_Byte[(unsigned int)font_file_size];
	in_stream.read((char*)font_file_data, font_file_size);

	FT_Error errors = 0;

	FT_Library library_freetype;

	errors = FT_Init_FreeType(&library_freetype);
	ASSERT_EQ(FT_Err_Ok, errors);

	FT_Face font_loaded = nullptr;
	errors = FT_New_Memory_Face(library_freetype, font_file_data, (FT_Long)font_file_size, 0, &font_loaded);
	ASSERT_EQ(FT_Err_Ok, errors);

	Library* library = new Library;

	GlyphProviderFreetype* provider = new GlyphProviderFreetype(library, font_loaded, font_file_data, (size_t)font_file_size);

	Face* face = provider->CreateFace(16.0f);
	GlyphBitmap* bitmap = face->GetGlyphBitmap(33);

	int i = 0;
}