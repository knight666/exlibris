#include "ExLibris.Test.PCH.h"

#include <Family.h>

#include "Mock.Font.h"

using namespace ExLibris;

TEST(Family, AddFont)
{
	Family* fam = new Family(nullptr, "Kittykat");

	MockFont* font = new MockFont("Kittykat");
	fam->AddFont(font);

	EXPECT_EQ(1, fam->GetFontCount());
}

TEST(Family, AddFontTwice)
{
	Family* fam = new Family(nullptr, "Bangers 'N Mash");

	MockFont* font = new MockFont("Bangers 'N Mash");
	fam->AddFont(font);
	fam->AddFont(font);

	EXPECT_EQ(1, fam->GetFontCount());
}

TEST(Family, FindFont)
{
	Family* fam = new Family(nullptr, "Veranda");

	MockFont* font = new MockFont("Veranda");
	font->SetWeight(eWeight_Bold);
	font->SetStyle(eStyle_None);

	fam->AddFont(font);

	IFont* font_found = fam->FindFont(eWeight_Bold, eStyle_None);
	EXPECT_EQ(font, font_found);
}

TEST(Family, FindFontMatchWeight)
{
	Family* fam = new Family(nullptr, "Droned");

	MockFont* font = new MockFont("Droned");
	font->SetWeight(eWeight_Bold);
	font->SetStyle(eStyle_Italicized);

	fam->AddFont(font);

	IFont* font_found = fam->FindFont(eWeight_Bold, eStyle_None);
	EXPECT_EQ(font, font_found);
}

TEST(Family, FindFontMatchStyle)
{
	Family* fam = new Family(nullptr, "BigBoned");

	MockFont* font = new MockFont("BigBoned");
	font->SetWeight(eWeight_Bold);
	font->SetStyle(eStyle_Italicized);

	fam->AddFont(font);

	IFont* font_found = fam->FindFont(eWeight_Normal, eStyle_Italicized);
	EXPECT_EQ(font, font_found);
}

TEST(Family, FindFontEmpty)
{
	Family* fam = new Family(nullptr, "Teacup");

	IFont* font_found = fam->FindFont(eWeight_Normal, eStyle_Italicized);
	ASSERT_EQ(nullptr, font_found);
}

TEST(Family, FindFontNotFound)
{
	Family* fam = new Family(nullptr, "Teacup");

	IFont* font_found = fam->FindFont(eWeight_Normal, eStyle_Italicized);
	ASSERT_EQ(nullptr, font_found);
}