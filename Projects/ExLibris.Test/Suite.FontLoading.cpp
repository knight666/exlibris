#include "ExLibris.Test.PCH.h"

#include "FontLoaderFreetype.h"

using namespace ExLibris;

TEST(FontLoaderFreetype, LoadFont)
{
	FontLoaderFreetype loader;

	FT_Face font_loaded = loader.LoadFontFace("Fonts/Roboto/Roboto-Regular.ttf");
	EXPECT_NE(nullptr, font_loaded);
}