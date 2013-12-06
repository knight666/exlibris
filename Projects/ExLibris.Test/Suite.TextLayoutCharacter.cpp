#include "ExLibris.Test.PCH.h"

#include <Face.h>
#include <TextFormat.h>
#include <TextLayoutCharacter.h>

#include "Mock.GlyphProvider.h"

using namespace ExLibris;

TEST(TextLayoutCharacter, Construct)
{
	Library lib;

	MockGlyphProvider* provider = new MockGlyphProvider(&lib);
	Face* face = provider->CreateFace(15.0f);

	TextFormat tf(&lib);
	tf.SetFace(face);

	TextLayoutCharacter* character = new TextLayoutCharacter(&tf, 'A');

	EXPECT_EQ(&tf, character->GetTextFormat());
	EXPECT_EQ('A', character->GetCodepoint());
	EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetElementGeometry().GetDimensions());
}

TEST(TextLayoutCharacter, CalculateGeometry)
{
	Library lib;

	MockGlyphProvider* provider = new MockGlyphProvider(&lib);
	Face* face = provider->CreateFace(24.0f);

	TextFormat tf(&lib);
	tf.SetFace(face);

	TextLayoutCharacter* character = new TextLayoutCharacter(&tf, '7');

	character->CalculateGeometry();

	EXPECT_VEC2_EQ(0.0f, 0.0f - ((24.0f / 2.0f) + -(24.0f / 3.0f)), character->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(12.0f, 24.0f * 1.5f, character->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(2.0f, 2.0f, character->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(8.0f, 8.0f, character->GetElementGeometry().GetDimensions());
}

TEST(TextLayoutCharacter, CalculateGeometryWithPosition)
{
	Library lib;

	MockGlyphProvider* provider = new MockGlyphProvider(&lib);
	Face* face = provider->CreateFace(24.0f);

	TextFormat tf(&lib);
	tf.SetFace(face);

	TextLayoutCharacter* character = new TextLayoutCharacter(&tf, 'P');
	character->SetPosition(glm::vec2(51.0f, 12.0f));

	character->CalculateGeometry();

	EXPECT_VEC2_EQ(51.0f, 12.0f - ((24.0f / 2.0f) + -(24.0f / 3.0f)), character->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(12.0f, 24.0f * 1.5f, character->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(51.0f + 2.0f, 12.0f + 2.0f, character->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(8.0f, 8.0f, character->GetElementGeometry().GetDimensions());
}

TEST(TextLayoutCharacter, CalculateGeometryWithKerningAdjustment)
{
	Library lib;

	MockGlyphProvider* provider = new MockGlyphProvider(&lib);
	Face* face = provider->CreateFace(33.0f);

	TextFormat tf(&lib);
	tf.SetFace(face);

	TextLayoutCharacter* character = new TextLayoutCharacter(&tf, '%');
	character->SetKerningAdjustment(glm::vec2(-2.4f, 15.2f));

	character->CalculateGeometry();

	EXPECT_VEC2_EQ(0.0f, 0.0f - ((33.0f / 2.0f) + -(33.0f / 3.0f)), character->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(12.0f - 2.4f, 33.0f * 1.5f, character->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(2.0f, 2.0f, character->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(8.0f, 8.0f, character->GetElementGeometry().GetDimensions());
}