#include "ExLibris.Test.PCH.h"

#include <FontFace.h>
#include <Glyph.h>

#include "Mock.Font.h"

using namespace ExLibris;

TEST(FontFace, Construct)
{
	FontFace* face = new FontFace(nullptr);

	EXPECT_EQ(nullptr, face->GetFont());
	EXPECT_FLOAT_EQ(0.0f, face->GetSize());
	EXPECT_FLOAT_EQ(0.0f, face->GetLineHeight());
	EXPECT_EQ(0, face->GetGlyphCount());
}

TEST(FontFace, ConstructFromFont)
{
	MockFont* font = new MockFont(nullptr);

	FontFace* face = font->CreateFace(12.0f);
	ASSERT_NE(nullptr, face);

	EXPECT_EQ(font, face->GetFont());
	EXPECT_FLOAT_EQ(12.0f, face->GetSize());
	EXPECT_FLOAT_EQ(24.0f, face->GetLineHeight());
	EXPECT_EQ(0, face->GetGlyphCount());
}

TEST(FontFace, AddGlyph)
{
	FontFace* face = new FontFace(nullptr);

	Glyph* glyph = new Glyph;
	glyph->index = 144;
	
	EXPECT_TRUE(face->AddGlyph(glyph));

	EXPECT_EQ(1, face->GetGlyphCount());
}

TEST(FontFace, AddGlyphTwice)
{
	FontFace* face = new FontFace(nullptr);

	Glyph* glyph = new Glyph;
	glyph->index = 33;
	
	EXPECT_TRUE(face->AddGlyph(glyph));
	EXPECT_FALSE(face->AddGlyph(glyph));

	EXPECT_EQ(1, face->GetGlyphCount());
}

TEST(FontFace, AddGlyphSameIndex)
{
	FontFace* face = new FontFace(nullptr);

	Glyph* glyph_first = new Glyph;
	glyph_first->index = 187;
	EXPECT_TRUE(face->AddGlyph(glyph_first));

	Glyph* glyph_second = new Glyph;
	glyph_second->index = 187;
	EXPECT_FALSE(face->AddGlyph(glyph_second));

	EXPECT_EQ(1, face->GetGlyphCount());
}

TEST(FontFace, FindGlyph)
{
	MockFont* font = new MockFont(nullptr);

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
	MockFont* font = new MockFont(nullptr);

	FontFace* face = new FontFace(font);

	Glyph* glyph = face->FindGlyph(32);
	ASSERT_EQ(nullptr, glyph);
}

TEST(FontFace, TryGetKerning)
{
	MockFont* font = new MockFont(nullptr);

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
	MockFont* font = new MockFont(nullptr);

	FontFace* face = new FontFace(font);

	Glyph* glyph = new Glyph;
	glyph->index = 166;

	Glyph* glyph_other = new Glyph;
	glyph_other->index = 78;

	face->AddGlyph(glyph);

	glm::vec2 kerning_actual;
	EXPECT_FALSE(face->TryGetKerning(glyph, glyph_other, kerning_actual));
}