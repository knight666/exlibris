#include "ExLibris.Test.PCH.h"

#include <Family.h>

#include "Mock.GlyphProvider.h"

using namespace ExLibris;

TEST(Family, Construct)
{
	Family* fam = new Family(nullptr, "Kittykat");

	EXPECT_STREQ("Kittykat", fam->GetName().c_str());
	EXPECT_EQ(0, fam->GetGlyphProviderCount());
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