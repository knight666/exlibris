#include "ExLibris.Test.PCH.h"

#include "FontLoaderFreetype.h"

using namespace ExLibris;

TEST(FontLoaderFreetype, LoadFont)
{
	FontLoaderFreetype loader;

	FontFreetype* font_loaded = loader.LoadFont("Fonts/Roboto/Roboto-Regular.ttf");
	EXPECT_NE(nullptr, font_loaded);

	EXPECT_STREQ("Roboto", font_loaded->GetFamilyName().c_str());
}

TEST(FontLoaderFreetype, LoadFontFileNotFound)
{
	FontLoaderFreetype loader;

	FontFreetype* font_loaded = loader.LoadFont("Fonts/Arial.ttf");
	EXPECT_EQ(nullptr, font_loaded);
}

TEST(FontLoaderFreetype, LoadFontNoData)
{
	FT_Face font_data = nullptr;

	FontFreetype* font = new FontFreetype("Bazinga!");
	EXPECT_FALSE(font->LoadFontData(font_data));
}

TEST(FontLoaderFreetype, LoadFontNoUnicode)
{
	FontLoaderFreetype loader;

	FT_Face font_data = new FT_FaceRec_;
	memset(font_data, 0, sizeof(FT_FaceRec_));

	FontFreetype* font = new FontFreetype("Shiny");
	EXPECT_FALSE(font->LoadFontData(font_data));
}