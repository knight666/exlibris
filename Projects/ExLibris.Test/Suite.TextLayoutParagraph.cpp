#include "ExLibris.Test.PCH.h"

#include <Face.h>
#include <TextLayoutParagraph.h>
#include <TextLayoutSection.h>

#include "Mock.GlyphProvider.h"

using namespace ExLibris;

TEST(TextLayoutParagraph, AddCharacter)
{
	MockGlyphProvider provider(nullptr);
	Face* face = provider.CreateFace(15.4f);

	TextLayoutParagraph* paragraph = new TextLayoutParagraph(face);

	paragraph->AddCharacter('H');

	EXPECT_EQ(1, paragraph->GetChildrenCount());

	TextLayoutSection* section = dynamic_cast<TextLayoutSection*>(paragraph->GetChildAtIndex(0));
	ASSERT_NE(nullptr, section);
}

TEST(TextLayoutParagraph, GeometryCharacter)
{
	MockGlyphProvider provider(nullptr);
	provider.use_kerning_pairs = true;

	Face* face = provider.CreateFace(11.5f);

	TextLayoutParagraph* paragraph = new TextLayoutParagraph(face);

	paragraph->AddCharacter('F');

	paragraph->CalculateGeometry();

	EXPECT_VEC2_EQ(0.0f, 0.0f - (11.5f / 2.0f) + -(11.5f / 3.0f), paragraph->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(12.0f, 11.5f * 1.5f, paragraph->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(2.0f, 2.0f, paragraph->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(8.0f, 8.0f, paragraph->GetElementGeometry().GetDimensions());
}

TEST(TextLayoutParagraph, GeometryFourCharacters)
{
	MockGlyphProvider provider(nullptr);
	provider.use_kerning_pairs = true;

	Face* face = provider.CreateFace(21.6f);

	TextLayoutParagraph* paragraph = new TextLayoutParagraph(face);

	paragraph->AddCharacter('F');
	paragraph->AddCharacter('i');
	paragraph->AddCharacter('s');
	paragraph->AddCharacter('h');

	paragraph->CalculateGeometry();

	EXPECT_VEC2_EQ(0.0f, 0.0f - (21.6f / 2.0f) + -(21.6f / 3.0f), paragraph->GetLayoutGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(12.0f * 4.0f, 21.6f * 1.5f, paragraph->GetLayoutGeometry().GetDimensions());
	EXPECT_VEC2_EQ(2.0f, 2.0f, paragraph->GetElementGeometry().GetTopLeft());
	EXPECT_VEC2_EQ(44.0f, 8.0f, paragraph->GetElementGeometry().GetDimensions());
}