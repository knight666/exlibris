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

		ADD_LOCALE(1043, eRtfCountry_Netherlands, eRtfLanguage_Dutch);
		ADD_LOCALE(1132, eRtfCountry_Netherlands, eRtfLanguage_Frisian);
		ADD_LOCALE(2067, eRtfCountry_Belgium, eRtfLanguage_Dutch);
		ADD_LOCALE(2060, eRtfCountry_Belgium, eRtfLanguage_French);

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