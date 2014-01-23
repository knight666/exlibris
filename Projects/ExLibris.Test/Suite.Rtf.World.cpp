#include "ExLibris.Test.PCH.h"

#include <RtfWorld.h>

using namespace ExLibris;

TEST(RtfWorld, GetLocaleByIdentifier)
{
	RtfWorld world;

	const RtfLocale* i = world.GetLocaleByIdentifier(1043);

	ASSERT_NE(nullptr, i);
	EXPECT_EQ(1043, i->identifier);
	EXPECT_EQ(Rtf::eCountry_Netherlands, i->country);
	EXPECT_EQ(Rtf::eLanguage_Dutch, i->language);
}

TEST(RtfWorld, GetLocaleByIdentifierInvalidIndex)
{
	RtfWorld world;

	const RtfLocale* i = world.GetLocaleByIdentifier(333);

	EXPECT_EQ(nullptr, i);
}

TEST(RtfWorld, GetLocaleByIdentifierZeroOrOneThousandTwentyFour)
{
	RtfWorld world;

	const RtfLocale* l0 = world.GetLocaleByIdentifier(0);

	ASSERT_NE(nullptr, l0);
	EXPECT_EQ(Rtf::eCountry_None, l0->country);
	EXPECT_EQ(Rtf::eLanguage_None, l0->language);

	const RtfLocale* l1024 = world.GetLocaleByIdentifier(1024);

	ASSERT_NE(nullptr, l1024);
	EXPECT_EQ(Rtf::eCountry_None, l1024->country);
	EXPECT_EQ(Rtf::eLanguage_None, l1024->language);
}

TEST(RtfWorld, GetLocaleIdentifierByLanguageAndCountry)
{
	RtfWorld world;

	unsigned int i = world.GetLocaleIdentifierByLanguageAndCountry(Rtf::eLanguage_Frisian, Rtf::eCountry_Netherlands);

	EXPECT_EQ(1132, i);
}

TEST(RtfWorld, GetLocaleIdentifierByLanguageNoneAndCountryNone)
{
	RtfWorld world;

	unsigned int i = world.GetLocaleIdentifierByLanguageAndCountry(Rtf::eLanguage_None, Rtf::eCountry_None);

	EXPECT_EQ(0, i);
}

TEST(RtfWorld, GetLanguagesForCountry)
{
	RtfWorld world;

	std::set<Rtf::Language> l = world.GetLanguagesForCountry(Rtf::eCountry_Netherlands);

	EXPECT_EQ(2, l.size());
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Dutch));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Frisian));
}

TEST(RtfWorld, GetLanguagesForCountryNone)
{
	RtfWorld world;

	std::set<Rtf::Language> l = world.GetLanguagesForCountry(Rtf::eCountry_None);

	EXPECT_EQ(23, l.size());
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_BosnianLatin));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Burmese));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Cherokee));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Dzongkha));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Fulfukle));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Guarani));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Hawaiian));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Kanuri));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_KashmiriArabic));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_KashmiriLatin));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Latin));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_LithuanianTrad));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Manipuri));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Oromo));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Papiamentu));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Persian));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_SindhiArabic));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_SindhiDevanagari));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Somali));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Tsonga));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Venda));
	EXPECT_NE(l.end(), l.find(Rtf::eLanguage_Yiddish));
}

TEST(RtfWorld, GetCountriesForLanguage)
{
	RtfWorld world;

	std::set<Rtf::Country> l = world.GetCountriesForLanguage(Rtf::eLanguage_Dutch);

	EXPECT_EQ(3, l.size());
	EXPECT_NE(l.end(), l.find(Rtf::eCountry_Preferred));
	EXPECT_NE(l.end(), l.find(Rtf::eCountry_Netherlands));
	EXPECT_NE(l.end(), l.find(Rtf::eCountry_Belgium));
}

TEST(RtfWorld, GetCountriesForLanguageNone)
{
	RtfWorld world;

	std::set<Rtf::Country> c = world.GetCountriesForLanguage(Rtf::eLanguage_None);

	EXPECT_EQ(1, c.size());
	EXPECT_NE(c.end(), c.find(Rtf::eCountry_None));
}

TEST(RtfWorld, GetCountriesForLanguageNoneFound)
{
	RtfWorld world;

	std::set<Rtf::Country> l = world.GetCountriesForLanguage(Rtf::eLanguage_Cherokee);

	EXPECT_EQ(1, l.size());
	EXPECT_NE(l.end(), l.find(Rtf::eCountry_None));
}