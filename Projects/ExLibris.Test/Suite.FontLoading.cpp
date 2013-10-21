#include "ExLibris.Test.PCH.h"

#include <Family.h>
#include <FontLoaderFreetype.h>
#include <Library.h>

using namespace ExLibris;

TEST(FontLoaderFreetype, LoadFont)
{
	Library lib;
	FontLoaderFreetype* loader = new FontLoaderFreetype(&lib);

	IFont* font_loaded = loader->LoadFont("Fonts/Roboto/Roboto-Regular.ttf");
	EXPECT_NE(nullptr, font_loaded);

	EXPECT_STREQ("Roboto", font_loaded->GetFamily()->GetName().c_str());
}

TEST(FontLoaderFreetype, LoadFontFileNotFound)
{
	Library lib;
	FontLoaderFreetype* loader = new FontLoaderFreetype(&lib);

	IFont* font_loaded = loader->LoadFont("Fonts/Arial.ttf");
	EXPECT_EQ(nullptr, font_loaded);
}