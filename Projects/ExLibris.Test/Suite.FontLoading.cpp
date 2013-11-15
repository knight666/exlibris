#include "ExLibris.Test.PCH.h"

#include <Exception.h>
#include <Family.h>
#include <FontFace.h>
#include <FontLoaderFreetype.h>
#include <Library.h>

using namespace ExLibris;

TEST(FontLoaderFreetype, LoadFont)
{
	Library lib;
	FontLoaderFreetype* loader = new FontLoaderFreetype(&lib);

	std::fstream in_stream("Fonts/Roboto/Roboto-Regular.ttf", std::ios::in | std::ios::binary);

	IFont* font_loaded = loader->LoadFont(in_stream);
	EXPECT_NE(nullptr, font_loaded);

	EXPECT_STREQ("Roboto", font_loaded->GetFamily()->GetName().c_str());
}

TEST(FontLoaderFreetype, LoadInvalidFile)
{
	Library lib;
	FontLoaderFreetype* loader = new FontLoaderFreetype(&lib);

	std::fstream in_stream("Tests/hello.txt", std::ios::in | std::ios::binary);

	EXPECT_THROW({
		IFont* font_loaded = loader->LoadFont(in_stream);
	}, Exception);
}

TEST(FontLoaderFreetype, LoadFontFileNotFound)
{
	Library lib;
	FontLoaderFreetype* loader = new FontLoaderFreetype(&lib);

	std::fstream in_stream("Fonts/Arial.ttf", std::ios::in | std::ios::binary);

	IFont* font_loaded = loader->LoadFont(in_stream);
	EXPECT_EQ(nullptr, font_loaded);
}

class FontFaceContext
	: public ::testing::Test
{

public:

	void SetUp()
	{
		library = new Library;
		library->AddLoader(new FontLoaderFreetype(library));

		font = (FontFreetype*)library->LoadFont("Fonts/Roboto/Roboto-Regular.ttf");

		FaceOptions options;
		options.size = 24.0f;

		face = font->CreateFace(options);
	}

	void TearDown()
	{
		delete library;
	}

protected:

	Library* library;
	FontFreetype* font;
	FontFace* face;

};

TEST_F(FontFaceContext, LineHeight)
{
	EXPECT_FLOAT_EQ(24.0f, face->GetSize());
	EXPECT_FLOAT_EQ(42.0f, face->GetLineHeight());
}

TEST_F(FontFaceContext, FindGlyph)
{
	Glyph* glyph = face->FindGlyph((unsigned int)'6');
	ASSERT_NE(nullptr, glyph);

	EXPECT_EQ(25, glyph->index);
	ASSERT_NE(nullptr, glyph->metrics);
	EXPECT_FLOAT_EQ(18.0f, glyph->metrics->advance);
	EXPECT_VEC2_EQ(2.0f, 19.0f, glyph->metrics->offset);
	EXPECT_VEC2_EQ(2.0625000f, 19.0f, glyph->metrics->bounding_box.GetMinimum());
	EXPECT_VEC2_EQ(16.859375f, 42.0f, glyph->metrics->bounding_box.GetMaximum());
}

TEST_F(FontFaceContext, FindGlyphOutline)
{
	Glyph* glyph = face->FindGlyph((unsigned int)'-');
	ASSERT_NE(nullptr, glyph);

	ASSERT_NE(nullptr, glyph->outline);
	EXPECT_EQ(5, glyph->outline->GetCommandCount());

	EXPECT_EQ(5, glyph->outline->GetPositionCount());
	EXPECT_VEC2_EQ(8.2031250f, -8.5937500f, glyph->outline->GetPosition(0));
}

TEST_F(FontFaceContext, FindGlyphNotFound)
{
	Glyph* glyph = face->FindGlyph(0x777126);
	ASSERT_EQ(nullptr, glyph);
}