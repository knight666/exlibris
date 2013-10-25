#include "ExLibris.Test.PCH.h"

#include <FontFace.h>
#include <FontFreetype.h>
#include <FontLoaderFreetype.h>
#include <Library.h>

using namespace ExLibris;

class FaceContext
	: public ::testing::Test
{

public:

	void SetUp()
	{
		library = new Library;
		library->AddLoader(new FontLoaderFreetype(library));

		font = (FontFreetype*)library->LoadFont("Fonts/Roboto/Roboto-Regular.ttf");

		face = font->CreateFace(24.0f);
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
	EXPECT_VEC2_EQ(2.0f, 19.0f, glyph->metrics->offset);
	EXPECT_VEC2_EQ(2.0625000f, 0.0000000f, glyph->metrics->bounding_box.minimum);
	EXPECT_VEC2_EQ(16.859375f, 23.000000f, glyph->metrics->bounding_box.maximum);
}

TEST_F(FaceContext, FindGlyphOutline)
{
	Glyph* glyph = face->FindGlyph((unsigned int)'-');
	ASSERT_NE(nullptr, glyph);

	ASSERT_NE(nullptr, glyph->outline);
	EXPECT_EQ(5, glyph->outline->GetCommandCount());

	EXPECT_EQ(5, glyph->outline->GetPositionCount());
	EXPECT_VEC2_EQ(8.2031250f, -8.5937500f, glyph->outline->GetPosition(0));
}

TEST_F(FaceContext, FindGlyphNotFound)
{
	Glyph* glyph = face->FindGlyph(0x777126);
	ASSERT_EQ(nullptr, glyph);
}