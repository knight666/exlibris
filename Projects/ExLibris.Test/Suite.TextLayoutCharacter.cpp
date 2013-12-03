#include "ExLibris.Test.PCH.h"

#include <Face.h>
#include <TextLayoutCharacter.h>

#include "Mock.GlyphProvider.h"

using namespace ExLibris;

TEST(TextLayoutCharacter, Construct)
{
	MockGlyphProvider provider(nullptr);
	Face* face = provider.CreateFace(15.0f);

	TextLayoutCharacter* character = new TextLayoutCharacter(face, 'A');

	EXPECT_EQ(face, character->GetFace());
	EXPECT_EQ('A', character->GetCodepoint());
	EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetElementGeometry().GetDimensions());
}

TEST(TextLayoutCharacter, CalculateGeometry)
{
	MockGlyphProvider provider(nullptr);
	Face* face = provider.CreateFace(24.0f);

	TextLayoutCharacter* character = new TextLayoutCharacter(face, '7');

	character->CalculateGeometry();

	EXPECT_VEC2_EQ(0.0f, 0.0f - ((24.0f / 2.0f) + -(24.0f / 3.0f)), character->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(12.0f, 24.0f * 1.5f, character->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(2.0f, 2.0f, character->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(8.0f, 8.0f, character->GetElementGeometry().GetDimensions());
}

TEST(TextLayoutCharacter, CalculateGeometryWithPosition)
{
	MockGlyphProvider provider(nullptr);
	Face* face = provider.CreateFace(24.0f);

	TextLayoutCharacter* character = new TextLayoutCharacter(face, 'P');
	character->SetPosition(glm::vec2(51.0f, 12.0f));

	character->CalculateGeometry();

	EXPECT_VEC2_EQ(51.0f, 12.0f - ((24.0f / 2.0f) + -(24.0f / 3.0f)), character->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(12.0f, 24.0f * 1.5f, character->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(51.0f + 2.0f, 12.0f + 2.0f, character->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(8.0f, 8.0f, character->GetElementGeometry().GetDimensions());
}

TEST(TextLayoutCharacter, CalculateGeometryWithKerningAdjustment)
{
	MockGlyphProvider provider(nullptr);
	Face* face = provider.CreateFace(33.0f);

	TextLayoutCharacter* character = new TextLayoutCharacter(face, '%');
	character->SetKerningAdjustment(glm::vec2(-2.4f, 15.2f));

	character->CalculateGeometry();

	EXPECT_VEC2_EQ(0.0f, 0.0f - ((33.0f / 2.0f) + -(33.0f / 3.0f)), character->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(12.0f - 2.4f, 33.0f * 1.5f, character->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(2.0f, 2.0f, character->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(8.0f, 8.0f, character->GetElementGeometry().GetDimensions());
}