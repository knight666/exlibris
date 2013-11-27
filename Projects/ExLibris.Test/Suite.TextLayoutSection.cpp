#include "ExLibris.Test.PCH.h"

#include <TextLayoutCharacter.h>
#include <TextLayoutSection.h>

#include "Mock.GlyphProvider.h"

using namespace ExLibris;

TEST(TextLayoutSection, AddCharacter)
{
	MockGlyphProvider provider(nullptr);

	Face* face = provider.CreateFace(31.9f);

	TextLayoutSection* section = new TextLayoutSection(face);

	TextLayoutCharacter* character = section->AddCharacter('A');

	EXPECT_EQ(1, section->GetChildrenCount());
}

TEST(TextLayoutSection, AddTwoCharactersWithKerning)
{
	MockGlyphProvider provider(nullptr);
	provider.use_kerning_pairs = true;

	MockGlyphProvider::KerningPair pair;
	pair.index_left = 'A';
	pair.index_right = 'V';
	pair.adjustment.x = -2.5f;
	pair.adjustment.y = 0.0f;
	provider.kerning_pairs.push_back(pair);

	Face* face = provider.CreateFace(14.0f);

	TextLayoutSection* section = new TextLayoutSection(face);

	TextLayoutCharacter* character_a = section->AddCharacter('A');
	TextLayoutCharacter* character_v = section->AddCharacter('V');

	EXPECT_VEC2_EQ(0.0f, 0.0f, character_a->GetPosition());
	EXPECT_VEC2_EQ(0.0f, 0.0f, character_a->GetKerningAdjustment());
	EXPECT_VEC2_EQ(12.0f, 0.0f, character_v->GetPosition());
	EXPECT_VEC2_EQ(-2.5f, 0.0f, character_v->GetKerningAdjustment());
}

TEST(TextLayoutSection, GeometryCharacter)
{
	MockGlyphProvider provider(nullptr);

	Face* face = provider.CreateFace(128.9f);

	TextLayoutSection* section = new TextLayoutSection(face);

	section->AddCharacter('B');

	section->CalculateGeometry();

	EXPECT_VEC2_EQ(0.0f, 0.0f - (128.9f / 2.0f) + -(128.9f / 3.0f), section->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(12.0f, 128.9f * 1.5f, section->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(2.0f, 2.0f, section->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(8.0f, 8.0f, section->GetElementGeometry().GetDimensions());
}

TEST(TextLayoutSection, GeometryThreeCharacters)
{
	MockGlyphProvider provider(nullptr);
	provider.use_kerning_pairs = true;

	Face* face = provider.CreateFace(57.2f);

	TextLayoutSection* section = new TextLayoutSection(face);

	section->AddCharacter('D');
	section->AddCharacter('O');
	section->AddCharacter('G');

	section->CalculateGeometry();

	EXPECT_VEC2_EQ(0.0f, 0.0f - (57.2f / 2.0f) + -(57.2f / 3.0f), section->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(12.0f * 3.0f, 57.2f * 1.5f, section->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(2.0f, 2.0f, section->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(8.0f * 3.0f + 10.0f - 2.0f, 8.0f, section->GetElementGeometry().GetDimensions());
}