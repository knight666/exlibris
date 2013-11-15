#include "ExLibris.Test.PCH.h"

#include <Family.h>

#include "Mock.Font.h"
#include "Mock.GlyphProvider.h"

using namespace ExLibris;

TEST(Family, AddFont)
{
	Family* fam = new Family(nullptr, "Kittykat");

	MockFont* font = new MockFont(fam);
	fam->AddFont(font);

	EXPECT_EQ(1, fam->GetFontCount());
}

TEST(Family, AddFontTwice)
{
	Family* fam = new Family(nullptr, "Bangers 'N Mash");

	MockFont* font = new MockFont(fam);
	fam->AddFont(font);
	fam->AddFont(font);

	EXPECT_EQ(1, fam->GetFontCount());
}

TEST(Family, FindFont)
{
	Family* fam = new Family(nullptr, "Veranda");

	MockFont* font = new MockFont(fam);
	font->SetWeight(eWeight_Bold);
	font->SetStyle(eStyle_None);

	fam->AddFont(font);

	IFont* font_found = fam->FindFont(eWeight_Bold, eStyle_None);
	EXPECT_EQ(font, font_found);
}

TEST(Family, FindFontMatchWeight)
{
	Family* fam = new Family(nullptr, "Droned");

	MockFont* font = new MockFont(fam);
	font->SetWeight(eWeight_Bold);
	font->SetStyle(eStyle_Italicized);

	fam->AddFont(font);

	IFont* font_found = fam->FindFont(eWeight_Bold, eStyle_None);
	EXPECT_EQ(font, font_found);
}

TEST(Family, FindFontMatchStyle)
{
	Family* fam = new Family(nullptr, "BigBoned");

	MockFont* font = new MockFont(fam);
	font->SetWeight(eWeight_Bold);
	font->SetStyle(eStyle_Italicized);

	fam->AddFont(font);

	IFont* font_found = fam->FindFont(eWeight_Normal, eStyle_Italicized);
	EXPECT_EQ(font, font_found);
}

TEST(Family, FindFontBestMatchWeight)
{
	Family* fam = new Family(nullptr, "Racketeer");

	MockFont* font_first = new MockFont(fam);
	font_first->SetWeight(eWeight_Normal);
	font_first->SetStyle(eStyle_None);
	fam->AddFont(font_first);

	MockFont* font_second = new MockFont(fam);
	font_second->SetWeight(eWeight_Bold);
	font_second->SetStyle(eStyle_None);
	fam->AddFont(font_second);

	IFont* font_found = fam->FindFont(eWeight_Bold, eStyle_None);
	EXPECT_EQ(font_second, font_found);
}

TEST(Family, FindFontBestMatchStyle)
{
	Family* fam = new Family(nullptr, "Hairy Head");

	MockFont* font_first = new MockFont(fam);
	font_first->SetWeight(eWeight_Normal);
	font_first->SetStyle(eStyle_Italicized);
	fam->AddFont(font_first);

	MockFont* font_second = new MockFont(fam);
	font_second->SetWeight(eWeight_Normal);
	font_second->SetStyle(eStyle_None);
	fam->AddFont(font_second);

	IFont* font_found = fam->FindFont(eWeight_Normal, eStyle_Italicized);
	EXPECT_EQ(font_first, font_found);
}

TEST(Family, FindFontPartialMatch)
{
	Family* fam = new Family(nullptr, "Ringading");

	MockFont* font = new MockFont(fam);
	font->SetWeight(eWeight_Bold);
	font->SetStyle(eStyle_None);
	fam->AddFont(font);

	IFont* font_found = fam->FindFont(eWeight_Bold, eStyle_Italicized);
	EXPECT_EQ(font, font_found);
}

TEST(Family, FindFontNoMatch)
{
	Family* fam = new Family(nullptr, "Destroyer of Cookies");

	MockFont* font = new MockFont(fam);
	font->SetWeight(eWeight_Normal);
	font->SetStyle(eStyle_None);
	fam->AddFont(font);

	IFont* font_found = fam->FindFont(eWeight_Bold, eStyle_Italicized);
	EXPECT_EQ(nullptr, font_found);
}

TEST(Family, FindFontEmpty)
{
	Family* fam = new Family(nullptr, "Teacup");

	IFont* font_found = fam->FindFont(eWeight_Normal, eStyle_Italicized);
	EXPECT_EQ(nullptr, font_found);
}

TEST(Family, AddGlyphProvider)
{
	Family* fam = new Family(nullptr, "Headphone");

	MockGlyphProvider* provider = new MockGlyphProvider(nullptr);
	fam->AddGlyphProvider(provider);

	EXPECT_EQ(1, fam->GetGlyphProviderCount());
}

TEST(Family, AddGlyphProviderTwice)
{
	Family* fam = new Family(nullptr, "Britain's Finest");

	MockGlyphProvider* provider = new MockGlyphProvider(nullptr);
	fam->AddGlyphProvider(provider);
	fam->AddGlyphProvider(provider);

	EXPECT_EQ(1, fam->GetGlyphProviderCount());
}

TEST(Family, AddGlyphProviderNull)
{
	Family* fam = new Family(nullptr, "Tweedy");

	MockGlyphProvider* provider = nullptr;
	fam->AddGlyphProvider(provider);

	EXPECT_EQ(0, fam->GetGlyphProviderCount());
}

TEST(Family, FindGlyphProvider)
{
	Family* fam = new Family(nullptr, "Bananaband");

	MockGlyphProvider* provider = new MockGlyphProvider(nullptr);
	provider->SetWeight(eWeight_Bold);
	provider->SetStyle(eStyle_Italicized);

	fam->AddGlyphProvider(provider);

	IGlyphProvider* found = fam->FindGlyphProvider(16.0f, eWeight_Bold, eStyle_Italicized);
	EXPECT_EQ(provider, found);
}

TEST(Family, FindGlyphProviderMatchWeight)
{
	Family* fam = new Family(nullptr, "PuppetShow");

	MockGlyphProvider* provider_first = new MockGlyphProvider(nullptr);
	provider_first->SetWeight(eWeight_Bold);
	provider_first->SetStyle(eStyle_None);
	fam->AddGlyphProvider(provider_first);

	MockGlyphProvider* provider_second = new MockGlyphProvider(nullptr);
	provider_second->SetWeight(eWeight_Normal);
	provider_second->SetStyle(eStyle_None);
	fam->AddGlyphProvider(provider_second);

	IGlyphProvider* found = fam->FindGlyphProvider(48.0f, eWeight_Bold, eStyle_None);
	EXPECT_EQ(provider_first, found);
}

TEST(Family, FindGlyphProviderMatchStyle)
{
	Family* fam = new Family(nullptr, "FieldConsole");

	MockGlyphProvider* provider_first = new MockGlyphProvider(nullptr);
	provider_first->SetWeight(eWeight_Normal);
	provider_first->SetStyle(eStyle_None);
	fam->AddGlyphProvider(provider_first);

	MockGlyphProvider* provider_second = new MockGlyphProvider(nullptr);
	provider_second->SetWeight(eWeight_Normal);
	provider_second->SetStyle(eStyle_Italicized);
	fam->AddGlyphProvider(provider_second);

	IGlyphProvider* found = fam->FindGlyphProvider(22.4f, eWeight_Normal, eStyle_Italicized);
	EXPECT_EQ(provider_second, found);
}

TEST(Family, FindGlyphProviderMatchWeightButNotStyle)
{
	Family* fam = new Family(nullptr, "Grafixor");

	MockGlyphProvider* provider_first = new MockGlyphProvider(nullptr);
	provider_first->SetWeight(eWeight_Bold);
	provider_first->SetStyle(eStyle_None);
	fam->AddGlyphProvider(provider_first);

	MockGlyphProvider* provider_second = new MockGlyphProvider(nullptr);
	provider_second->SetWeight(eWeight_Normal);
	provider_second->SetStyle(eStyle_Italicized);
	fam->AddGlyphProvider(provider_second);

	IGlyphProvider* found = fam->FindGlyphProvider(55.68f, eWeight_Bold, eStyle_Italicized);
	EXPECT_EQ(provider_first, found);
}

TEST(Family, FindGlyphProviderMatchStyleButNotWeight)
{
	Family* fam = new Family(nullptr, "Megabonus");

	MockGlyphProvider* provider_first = new MockGlyphProvider(nullptr);
	provider_first->SetWeight(eWeight_Normal);
	provider_first->SetStyle(eStyle_Italicized);
	fam->AddGlyphProvider(provider_first);

	MockGlyphProvider* provider_second = new MockGlyphProvider(nullptr);
	provider_second->SetWeight(eWeight_Bold);
	provider_second->SetStyle(eStyle_None);
	fam->AddGlyphProvider(provider_second);

	IGlyphProvider* found = fam->FindGlyphProvider(14.5f, eWeight_Normal, eStyle_None);
	EXPECT_EQ(provider_first, found);
}

TEST(Family, FindGlyphProviderMatchSizeNotAvailable)
{
	Family* fam = new Family(nullptr, "Paperthin");

	MockGlyphProvider* provider_first = new MockGlyphProvider(nullptr);
	provider_first->size_blacklist.insert(18.2f);
	provider_first->SetWeight(eWeight_Normal);
	provider_first->SetStyle(eStyle_Italicized);
	fam->AddGlyphProvider(provider_first);

	MockGlyphProvider* provider_second = new MockGlyphProvider(nullptr);
	provider_second->SetWeight(eWeight_Bold);
	provider_second->SetStyle(eStyle_None);
	fam->AddGlyphProvider(provider_second);

	IGlyphProvider* found = fam->FindGlyphProvider(18.2f, eWeight_Normal, eStyle_Italicized);
	EXPECT_EQ(provider_second, found);
}