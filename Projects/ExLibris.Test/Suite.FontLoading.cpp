#include "ExLibris.Test.PCH.h"

#include <FontLoaderFreetype.h>
#include <Library.h>

using namespace ExLibris;

TEST(FontLoaderFreetype, LoadFont)
{
	Library lib;
	FontLoaderFreetype loader(&lib);

	FontFreetype* font_loaded = loader.LoadFont("Fonts/Roboto/Roboto-Regular.ttf");
	EXPECT_NE(nullptr, font_loaded);

	EXPECT_STREQ("Roboto", font_loaded->GetFamily()->GetName().c_str());
}

TEST(FontLoaderFreetype, LoadFontFileNotFound)
{
	Library lib;
	FontLoaderFreetype loader(&lib);

	FontFreetype* font_loaded = loader.LoadFont("Fonts/Arial.ttf");
	EXPECT_EQ(nullptr, font_loaded);
}

TEST(FontLoaderFreetype, LoadFontNoData)
{
	FT_Face font_data = nullptr;

	FontFreetype* font = new FontFreetype(nullptr);
	EXPECT_FALSE(font->LoadFontData(font_data));
}

TEST(FontLoaderFreetype, LoadFontNoUnicode)
{
	Library lib;
	FontLoaderFreetype loader(&lib);

	FT_Face font_data = new FT_FaceRec_;
	memset(font_data, 0, sizeof(FT_FaceRec_));

	FontFreetype* font = new FontFreetype(nullptr);
	EXPECT_FALSE(font->LoadFontData(font_data));
}