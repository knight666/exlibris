#include "ExLibris.Test.PCH.h"

#include <Exception.h>
#include <Family.h>
#include <FontFace.h>
#include <FontLoaderFreetype.h>
#include <IGlyphProvider.h>
#include <Library.h>

using namespace ExLibris;

TEST(FontLoaderFreetype, LoadGlyphProvider)
{
	Library lib;
	FontLoaderFreetype* loader = new FontLoaderFreetype(&lib);

	std::fstream in_stream("Fonts/Roboto/Roboto-Regular.ttf", std::ios::in | std::ios::binary);
	ASSERT_TRUE(in_stream.is_open());

	IGlyphProvider* provider = loader->LoadGlyphProvider(in_stream);
	EXPECT_NE(nullptr, provider);

	ASSERT_NE(nullptr, provider->GetFamily());
	EXPECT_STREQ("Roboto", provider->GetFamily()->GetName().c_str());
	EXPECT_EQ(eStyle_None, provider->GetStyle());
	EXPECT_EQ(eWeight_Normal, provider->GetWeight());
}

TEST(FontLoaderFreetype, LoadProviderInvalid)
{
	Library lib;
	FontLoaderFreetype* loader = new FontLoaderFreetype(&lib);

	std::fstream in_stream("Tests/hello.txt", std::ios::in | std::ios::binary);
	ASSERT_TRUE(in_stream.is_open());

	EXPECT_THROW({
		IGlyphProvider* provider = loader->LoadGlyphProvider(in_stream);
	}, Exception);
}

TEST(FontLoaderFreetype, LoadProviderFileNotFound)
{
	Library lib;
	FontLoaderFreetype* loader = new FontLoaderFreetype(&lib);

	std::fstream in_stream("Fonts/Arial.ttf", std::ios::in | std::ios::binary);
	ASSERT_FALSE(in_stream.is_open());

	IGlyphProvider* provider = loader->LoadGlyphProvider(in_stream);
	EXPECT_EQ(nullptr, provider);
}