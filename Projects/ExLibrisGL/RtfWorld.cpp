/*
 * This file is a part of the ExLibris project.
 *
 * Copyright (C) 2013 Quinten Lansu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

#include "ExLibrisGL.PCH.h"

#include "RtfWorld.h"

namespace ExLibris
{

	RtfWorld::RtfWorld()
	{
#define ADD_LOCALE(_id, _country, _language) m_Locales.insert(std::make_pair(_id, new RtfLocale(_id, _country, _language)));

		// static const RtfLanguagePair g_Language{:Lu:Ll+}{:Lu.#} = {.#};
		// ADD_LOCALE(\3, eRtfCountry_\2, eRtfLanguage_\1);

		ADD_LOCALE(1078, eRtfCountry_SouthAfrica, eRtfLanguage_Afrikaans);
		ADD_LOCALE(1052, eRtfCountry_Albania, eRtfLanguage_Albanian);
		ADD_LOCALE(1156, eRtfCountry_France, eRtfLanguage_Alsatian);
		ADD_LOCALE(1118, eRtfCountry_Ethiopia, eRtfLanguage_Amharic);
		ADD_LOCALE(5121, eRtfCountry_Algeria, eRtfLanguage_Arabic);
		ADD_LOCALE(15361, eRtfCountry_Bahrain, eRtfLanguage_Arabic);
		ADD_LOCALE(3073, eRtfCountry_Egypt, eRtfLanguage_Arabic);
		ADD_LOCALE(2049, eRtfCountry_Iraq, eRtfLanguage_Arabic);
		ADD_LOCALE(11265, eRtfCountry_Jordan, eRtfLanguage_Arabic);
		ADD_LOCALE(13313, eRtfCountry_Kuwait, eRtfLanguage_Arabic);
		ADD_LOCALE(12289, eRtfCountry_Lebanon, eRtfLanguage_Arabic);
		ADD_LOCALE(4097, eRtfCountry_Libya, eRtfLanguage_Arabic);
		ADD_LOCALE(6145, eRtfCountry_Morocco, eRtfLanguage_Arabic);
		ADD_LOCALE(8193, eRtfCountry_Oman, eRtfLanguage_Arabic);
		ADD_LOCALE(16385, eRtfCountry_Qatar, eRtfLanguage_Arabic);
		ADD_LOCALE(1025, eRtfCountry_SaudiArabia, eRtfLanguage_Arabic);
		ADD_LOCALE(10241, eRtfCountry_Syria, eRtfLanguage_Arabic);
		ADD_LOCALE(7169, eRtfCountry_Tunisia, eRtfLanguage_Arabic);
		ADD_LOCALE(14337, eRtfCountry_UnitedArabicEmirates, eRtfLanguage_Arabic);
		ADD_LOCALE(9217, eRtfCountry_Yemen, eRtfLanguage_Arabic);
		ADD_LOCALE(1067, eRtfCountry_Armenia, eRtfLanguage_Armenian);
		ADD_LOCALE(1101, eRtfCountry_India, eRtfLanguage_Assamese);
		ADD_LOCALE(2092, eRtfCountry_Azerbaijan, eRtfLanguage_AzeriCyrillic);
		ADD_LOCALE(1068, eRtfCountry_Azerbaijan, eRtfLanguage_AzeriLatin);
		ADD_LOCALE(1133, eRtfCountry_Russia, eRtfLanguage_Bashkir);
		ADD_LOCALE(1069, eRtfCountry_Basque, eRtfLanguage_Basque);
		ADD_LOCALE(1059, eRtfCountry_Belarus, eRtfLanguage_Belarusian);
		ADD_LOCALE(2117, eRtfCountry_Bangladesh, eRtfLanguage_Bengali);
		ADD_LOCALE(1093, eRtfCountry_India, eRtfLanguage_Bengali);
		ADD_LOCALE(4122, eRtfCountry_None, eRtfLanguage_BosnianLatin);
		ADD_LOCALE(8218, eRtfCountry_BosniaHerzegovina, eRtfLanguage_BosnianCyrillic);
		ADD_LOCALE(5146, eRtfCountry_BosniaHerzegovina, eRtfLanguage_BosnianLatin);
		ADD_LOCALE(1150, eRtfCountry_France, eRtfLanguage_Breton);
		ADD_LOCALE(1026, eRtfCountry_Bulgaria, eRtfLanguage_Bulgarian);
		ADD_LOCALE(1109, eRtfCountry_None, eRtfLanguage_Burmese);
		ADD_LOCALE(1027, eRtfCountry_Catalan, eRtfLanguage_Catalan);
		ADD_LOCALE(1116, eRtfCountry_None, eRtfLanguage_Cherokee);
		ADD_LOCALE(3076, eRtfCountry_SpecialAdministrativeRegionHongKong, eRtfLanguage_Chinese);
		ADD_LOCALE(5124, eRtfCountry_SpecialAdministrativeRegionMacao, eRtfLanguage_Chinese);
		ADD_LOCALE(2052, eRtfCountry_ChinaPeoplesRepublicOf, eRtfLanguage_Chinese);
		ADD_LOCALE(4100, eRtfCountry_Singapore, eRtfLanguage_Chinese);
		ADD_LOCALE(1028, eRtfCountry_Taiwan, eRtfLanguage_Chinese);
		ADD_LOCALE(1155, eRtfCountry_France, eRtfLanguage_Corsican);
		ADD_LOCALE(1050, eRtfCountry_Croatia, eRtfLanguage_Croatian);
		ADD_LOCALE(4122, eRtfCountry_BosniaHerzegovina, eRtfLanguage_CroatianLatin);
		ADD_LOCALE(3072, eRtfCountry_Current, eRtfLanguage_Custom);
		ADD_LOCALE(1029, eRtfCountry_CzechRepublic, eRtfLanguage_Czech);
		ADD_LOCALE(1030, eRtfCountry_Denmark, eRtfLanguage_Danish);
		ADD_LOCALE(1164, eRtfCountry_Afghanistan, eRtfLanguage_Dari);
		ADD_LOCALE(1125, eRtfCountry_Maldives, eRtfLanguage_Divehi);
		ADD_LOCALE(2067, eRtfCountry_Belgium, eRtfLanguage_Dutch);
		ADD_LOCALE(1043, eRtfCountry_Netherlands, eRtfLanguage_Dutch);
		ADD_LOCALE(19, eRtfCountry_Preferred, eRtfLanguage_Dutch);
		ADD_LOCALE(1126, eRtfCountry_None, eRtfLanguage_Dzongkha);
		ADD_LOCALE(3081, eRtfCountry_Australia, eRtfLanguage_English);
		ADD_LOCALE(10249, eRtfCountry_Belize, eRtfLanguage_English);
		ADD_LOCALE(4105, eRtfCountry_Canada, eRtfLanguage_English);
		ADD_LOCALE(9225, eRtfCountry_Caribbean, eRtfLanguage_English);
		ADD_LOCALE(15369, eRtfCountry_SpecialAdministrativeRegionHongKong, eRtfLanguage_English);
		ADD_LOCALE(16393, eRtfCountry_India, eRtfLanguage_English);
		ADD_LOCALE(14345, eRtfCountry_Indonesia, eRtfLanguage_English);
		ADD_LOCALE(6153, eRtfCountry_Ireland, eRtfLanguage_English);
		ADD_LOCALE(8201, eRtfCountry_Jamaica, eRtfLanguage_English);
		ADD_LOCALE(17417, eRtfCountry_Malaysia, eRtfLanguage_English);
		ADD_LOCALE(5129, eRtfCountry_NewZealand, eRtfLanguage_English);
		ADD_LOCALE(13321, eRtfCountry_Philippines, eRtfLanguage_English);
		ADD_LOCALE(18441, eRtfCountry_Singapore, eRtfLanguage_English);
		ADD_LOCALE(7177, eRtfCountry_SouthAfrica, eRtfLanguage_English);
		ADD_LOCALE(11273, eRtfCountry_TrinidadAndTobago, eRtfLanguage_English);
		ADD_LOCALE(2057, eRtfCountry_UnitedKingdom, eRtfLanguage_English);
		ADD_LOCALE(1033, eRtfCountry_UnitedStates, eRtfLanguage_English);
		ADD_LOCALE(11297, eRtfCountry_Zimbabwe, eRtfLanguage_English);
		ADD_LOCALE(1061, eRtfCountry_Estonia, eRtfLanguage_Estonian);
		ADD_LOCALE(1080, eRtfCountry_FaroeIslands, eRtfLanguage_Faroese);
		ADD_LOCALE(1124, eRtfCountry_Philippines, eRtfLanguage_Filipino);
		ADD_LOCALE(1035, eRtfCountry_Finland, eRtfLanguage_Finnish);
		ADD_LOCALE(2060, eRtfCountry_Belgium, eRtfLanguage_French);
		ADD_LOCALE(11276, eRtfCountry_Cameroon, eRtfLanguage_French);
		ADD_LOCALE(3084, eRtfCountry_Canada, eRtfLanguage_French);
		ADD_LOCALE(9228, eRtfCountry_CongoDemocraticRepublicOf, eRtfLanguage_French);
		ADD_LOCALE(12300, eRtfCountry_CotedIvoire, eRtfLanguage_French);
		ADD_LOCALE(1036, eRtfCountry_France, eRtfLanguage_French);
		ADD_LOCALE(15372, eRtfCountry_Haiti, eRtfLanguage_French);
		ADD_LOCALE(5132, eRtfCountry_Luxembourg, eRtfLanguage_French);
		ADD_LOCALE(13324, eRtfCountry_Mali, eRtfLanguage_French);
		ADD_LOCALE(6156, eRtfCountry_Monaco, eRtfLanguage_French);
		ADD_LOCALE(14348, eRtfCountry_Morocco, eRtfLanguage_French);
		ADD_LOCALE(8204, eRtfCountry_Reunion, eRtfLanguage_French);
		ADD_LOCALE(10252, eRtfCountry_Senegal, eRtfLanguage_French);
		ADD_LOCALE(4108, eRtfCountry_Switzerland, eRtfLanguage_French);
		ADD_LOCALE(7180, eRtfCountry_WestIndies, eRtfLanguage_French);
		ADD_LOCALE(1132, eRtfCountry_Netherlands, eRtfLanguage_Frisian);
		ADD_LOCALE(1127, eRtfCountry_None, eRtfLanguage_Fulfukle);
		ADD_LOCALE(2108, eRtfCountry_Ireland, eRtfLanguage_Gaelic);

#undef ADD_IDENTIFIER
	}

	RtfWorld::~RtfWorld()
	{
		for (std::map<unsigned int, RtfLocale*>::iterator entry_it = m_Locales.begin(); entry_it != m_Locales.end(); ++entry_it)
		{
			delete entry_it->second;
		}
		m_Locales.clear();
	}

	RtfLocale* RtfWorld::GetLocaleByIdentifier(unsigned int a_Index) const
	{
		std::map<unsigned int, RtfLocale*>::const_iterator found = m_Locales.find(a_Index);
		if (found != m_Locales.end())
		{
			return found->second;
		}
		else
		{
			return nullptr;
		}
	}

	unsigned int RtfWorld::GetLocaleIdentifierByLanguageAndCountry(RtfLanguage a_Language, RtfCountry a_Country) const
	{
		for (std::map<unsigned int, RtfLocale*>::const_iterator entry_it = m_Locales.begin(); entry_it != m_Locales.end(); ++entry_it)
		{
			RtfLocale* entry = entry_it->second;
			if (entry->country == a_Country && entry->language == a_Language)
			{
				return entry->identifier;
			}
		}

		return 0;
	}

	std::set<RtfLanguage> RtfWorld::GetLanguagesForCountry(RtfCountry a_Country) const
	{
		std::set<RtfLanguage> results;

		for (std::map<unsigned int, RtfLocale*>::const_iterator entry_it = m_Locales.begin(); entry_it != m_Locales.end(); ++entry_it)
		{
			RtfLocale* entry = entry_it->second;
			if (entry->country == a_Country)
			{
				results.insert(entry->language);
			}
		}

		return results;
	}

	std::set<RtfCountry> RtfWorld::GetCountriesForLanguage(RtfLanguage a_Language) const
	{
		std::set<RtfCountry> results;

		for (std::map<unsigned int, RtfLocale*>::const_iterator entry_it = m_Locales.begin(); entry_it != m_Locales.end(); ++entry_it)
		{
			RtfLocale* entry = entry_it->second;
			if (entry->language == a_Language)
			{
				results.insert(entry->country);
			}
		}

		return results;
	}

}; // namespace ExLibris