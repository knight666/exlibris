#include "ExLibris.Test.PCH.h"

#include "FontFreetype.h"
#include "FontFaceFreetype.h"

using namespace ExLibris;

TEST(FontFaceFreetype, Construct)
{
	FontFaceFreetype* face = new FontFaceFreetype(nullptr);

	EXPECT_EQ(nullptr, face->GetFont());
	EXPECT_FLOAT_EQ(0.0f, face->GetSize());
}

TEST(FontFaceFreetype, ConstructFromFont)
{
	FontFreetype* font = new FontFreetype("Broom 2.0");
	FontFaceFreetype* face = new FontFaceFreetype(font);

	EXPECT_EQ(font, face->GetFont());
}