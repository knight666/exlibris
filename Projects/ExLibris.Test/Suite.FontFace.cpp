#include "ExLibris.Test.PCH.h"

#include "FontFace.h"
#include "FontFreetype.h"
#include "FontLoaderFreetype.h"
#include "Glyph.h"

using namespace ExLibris;

TEST(FontFace, Construct)
{
	FontFace* face = new FontFace(nullptr);

	EXPECT_EQ(nullptr, face->GetFont());
	EXPECT_FLOAT_EQ(0.0f, face->GetSize());
	EXPECT_FLOAT_EQ(0.0f, face->GetLineHeight());
}

TEST(FontFace, ConstructFromFont)
{
	FontFreetype* font = new FontFreetype("Broom 2.0");
	FontFace* face = new FontFace(font);

	EXPECT_EQ(font, face->GetFont());
}

TEST(FontFace, FindGlyphNoFontEmpty)
{
	FontFace* face = new FontFace(nullptr);

	Glyph* glyph = face->FindGlyph(32);
	ASSERT_EQ(nullptr, glyph);
}

class FaceContext
	: public ::testing::Test
{

public:

	void SetUp()
	{
		loader = new FontLoaderFreetype;
		font = loader->LoadFont("Fonts/Roboto/Roboto-Regular.ttf");
		face = font->CreateFace(24.0f);
	}

	void TearDown()
	{
		delete face;
		delete font;
		delete loader;
	}

protected:

	FontLoaderFreetype* loader;
	FontFreetype* font;
	FontFace* face;

};

TEST_F(FaceContext, LineHeight)
{
	EXPECT_FLOAT_EQ(24.0f, face->GetSize());
	EXPECT_FLOAT_EQ(42.0f, face->GetLineHeight());
}

TEST_F(FaceContext, FindGlyph)
{
	Glyph* glyph = face->FindGlyph((unsigned int)'6');
	ASSERT_NE(nullptr, glyph);

	EXPECT_EQ(25, glyph->index);
	ASSERT_NE(nullptr, glyph->metrics);
	EXPECT_FLOAT_EQ(18.0f, glyph->metrics->advance);
	EXPECT_FLOAT_EQ(2.0f, glyph->metrics->offset.x);
	EXPECT_FLOAT_EQ(19.0f, glyph->metrics->offset.y);
}

TEST_F(FaceContext, FindGlyphNotFound)
{
	Glyph* glyph = face->FindGlyph(0x777126);
	ASSERT_EQ(nullptr, glyph);
}