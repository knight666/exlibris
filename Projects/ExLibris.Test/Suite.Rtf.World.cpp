#include "ExLibris.Test.PCH.h"

#include <RtfWorld.h>

using namespace ExLibris;

TEST(RtfWorld, GetLocaleByIdentifier)
{
	RtfWorld world;

	RtfLocale* i = world.GetLocaleByIdentifier(1043);

	ASSERT_NE(nullptr, i);
	EXPECT_EQ(1043, i->identifier);
	EXPECT_EQ(eRtfCountry_Netherlands, i->country);
	EXPECT_EQ(eRtfLanguage_Dutch, i->language);
}

TEST(RtfWorld, GetLocaleByIdentifierInvalidIndex)
{
	RtfWorld world;

	RtfLocale* i = world.GetLocaleByIdentifier(333);

	EXPECT_EQ(nullptr, i);
}

TEST(RtfWorld, GetLocaleIdentifierByLanguageAndCountry)
{
	RtfWorld world;

	unsigned int i = world.GetLocaleIdentifierByLanguageAndCountry(eRtfLanguage_Frisian, eRtfCountry_Netherlands);

	EXPECT_EQ(1132, i);
}

TEST(RtfWorld, GetLanguagesForCountry)
{
	RtfWorld world;

	std::set<RtfLanguage> l = world.GetLanguagesForCountry(eRtfCountry_Netherlands);

	EXPECT_EQ(2, l.size());
	EXPECT_NE(l.end(), l.find(eRtfLanguage_Dutch));
	EXPECT_NE(l.end(), l.find(eRtfLanguage_Frisian));
}

TEST(RtfWorld, GetLanguagesForCountryNoneFound)
{
	RtfWorld world;

	std::set<RtfLanguage> l = world.GetLanguagesForCountry(eRtfCountry_None);

	EXPECT_EQ(0, l.size());
}

TEST(RtfWorld, GetCountriesForLanguage)
{
	RtfWorld world;

	std::set<RtfCountry> l = world.GetCountriesForLanguage(eRtfLanguage_Dutch);

	EXPECT_EQ(2, l.size());
	EXPECT_NE(l.end(), l.find(eRtfCountry_Netherlands));
	EXPECT_NE(l.end(), l.find(eRtfCountry_Belgium));
}

TEST(RtfWorld, GetCountriesForLanguageNoneFound)
{
	RtfWorld world;

	std::set<RtfCountry> l = world.GetCountriesForLanguage(eRtfLanguage_None);

	EXPECT_EQ(0, l.size());
}