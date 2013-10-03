#include "ExLibris.Test.PCH.h"

#include <FontFace.h>
#include <FontFreetype.h>
#include <FontLoaderFreetype.h>
#include <Glyph.h>

#include "Mock.Font.h"

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
	MockFont* font = new MockFont("Broom 2.0");

	FontFace* face = font->CreateFace(12.0f);
	ASSERT_NE(nullptr, face);

	EXPECT_EQ(font, face->GetFont());
	EXPECT_FLOAT_EQ(12.0f, face->GetSize());
	EXPECT_FLOAT_EQ(24.0f, face->GetLineHeight());
}

TEST(FontFace, FindGlyph)
{
	MockFont* font = new MockFont("Best Greetings");

	FontFace* face = new FontFace(font);

	Glyph* glyph = new Glyph;
	glyph->index = 25;
	face->AddGlyph(glyph);

	Glyph* glyph_found = face->FindGlyph(25);
	ASSERT_EQ(glyph, glyph_found);
}

TEST(FontFace, FindGlyphNoFont)
{
	FontFace* face = new FontFace(nullptr);

	Glyph* glyph = face->FindGlyph(32);
	ASSERT_EQ(nullptr, glyph);
}

TEST(FontFace, FindGlyphEmpty)
{
	MockFont* font = new MockFont("Smexy");
	FontFace* face = new FontFace(font);

	Glyph* glyph = face->FindGlyph(32);
	ASSERT_EQ(nullptr, glyph);
}

TEST(FontFace, TryGetKerning)
{
	MockFont* font = new MockFont("Elderberries 2.0");
	FontFace* face = new FontFace(font);

	Glyph* glyph = new Glyph;
	glyph->index = 1;

	Glyph* glyph_other = new Glyph;
	glyph_other->index = 2;

	face->AddGlyph(glyph);
	face->AddGlyph(glyph_other);

	glm::vec2 kerning_expected(-4.5f, 22.0f);
	glyph->kernings.insert(std::make_pair(glyph_other->index, kerning_expected));

	glm::vec2 kerning_actual;
	EXPECT_TRUE(face->TryGetKerning(glyph, glyph_other, kerning_actual));

	EXPECT_VEC2_EQ(-4.5f, 22.0f, kerning_actual);
}

TEST(FontFace, TryGetKerningNotFound)
{
	MockFont* font = new MockFont("Roller");
	FontFace* face = new FontFace(font);

	Glyph* glyph = new Glyph;
	glyph->index = 166;

	Glyph* glyph_other = new Glyph;
	glyph_other->index = 78;

	face->AddGlyph(glyph);

	glm::vec2 kerning_actual;
	EXPECT_FALSE(face->TryGetKerning(glyph, glyph_other, kerning_actual));
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
	EXPECT_VEC2_EQ(2.0f, 34.0f, glyph->metrics->offset);
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