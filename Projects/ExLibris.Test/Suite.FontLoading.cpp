#include "ExLibris.Test.PCH.h"

#include "FontLoaderFreetype.h"

using namespace ExLibris;

TEST(FontLoaderFreetype, LoadFont)
{
	FontLoaderFreetype loader;

	FontFreetype* font_loaded = loader.LoadFontFace("Fonts/Roboto/Roboto-Regular.ttf");
	EXPECT_NE(nullptr, font_loaded);

	EXPECT_STREQ("Roboto", font_loaded->GetFamilyName().c_str());
}

TEST(FontLoaderFreetype, LoadFontFileNotFound)
{
	FontLoaderFreetype loader;

	FontFreetype* font_loaded = loader.LoadFontFace("Fonts/Arial.ttf");
	EXPECT_EQ(nullptr, font_loaded);
}