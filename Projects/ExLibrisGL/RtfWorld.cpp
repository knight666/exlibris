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
		ADD_LOCALE(8218, eRtfCountry_BosniaAndHerzegovina, eRtfLanguage_BosnianCyrillic);
		ADD_LOCALE(5146, eRtfCountry_BosniaAndHerzegovina, eRtfLanguage_BosnianLatin);
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
		ADD_LOCALE(4122, eRtfCountry_BosniaAndHerzegovina, eRtfLanguage_CroatianLatin);
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
		ADD_LOCALE(1084, eRtfCountry_Scotland, eRtfLanguage_Gaelic);
		ADD_LOCALE(1079, eRtfCountry_Georgia, eRtfLanguage_Georgian);
		ADD_LOCALE(3079, eRtfCountry_Austria, eRtfLanguage_German);
		ADD_LOCALE(1031, eRtfCountry_Germany, eRtfLanguage_German);
		ADD_LOCALE(5127, eRtfCountry_Liechtenstein, eRtfLanguage_German);
		ADD_LOCALE(4103, eRtfCountry_Luxembourg, eRtfLanguage_German);
		ADD_LOCALE(2055, eRtfCountry_Switzerland, eRtfLanguage_German);
		ADD_LOCALE(1032, eRtfCountry_Greece, eRtfLanguage_Greek);
		ADD_LOCALE(1135, eRtfCountry_Greenland, eRtfLanguage_Greenlandic);
		ADD_LOCALE(1140, eRtfCountry_None, eRtfLanguage_Guarani);
		ADD_LOCALE(1095, eRtfCountry_India, eRtfLanguage_Gujarati);
		ADD_LOCALE(1128, eRtfCountry_Nigeria, eRtfLanguage_HausaLatin);
		ADD_LOCALE(1141, eRtfCountry_None, eRtfLanguage_Hawaiian);
		ADD_LOCALE(1037, eRtfCountry_Israel, eRtfLanguage_Hebrew);
		ADD_LOCALE(1081, eRtfCountry_India, eRtfLanguage_Hindi);
		ADD_LOCALE(1038, eRtfCountry_Hungary, eRtfLanguage_Hungarian);
		ADD_LOCALE(1129, eRtfCountry_Nigeria, eRtfLanguage_Ibibio);
		ADD_LOCALE(1039, eRtfCountry_Iceland, eRtfLanguage_Icelandic);
		ADD_LOCALE(1136, eRtfCountry_Nigeria, eRtfLanguage_Igbo);
		ADD_LOCALE(2141, eRtfCountry_Canada, eRtfLanguage_InuktitutLatin);
		ADD_LOCALE(1117, eRtfCountry_Canada, eRtfLanguage_InuktitutSyllabics);
		ADD_LOCALE(1076, eRtfCountry_SouthAfrica, eRtfLanguage_IsiXhosa);
		ADD_LOCALE(1077, eRtfCountry_SouthAfrica, eRtfLanguage_IsiZulu);
		ADD_LOCALE(1040, eRtfCountry_Italy, eRtfLanguage_Italian);
		ADD_LOCALE(2064, eRtfCountry_Switzerland, eRtfLanguage_Italian);
		ADD_LOCALE(1041, eRtfCountry_Japan, eRtfLanguage_Japanese);
		ADD_LOCALE(1099, eRtfCountry_India, eRtfLanguage_Kannada);
		ADD_LOCALE(1137, eRtfCountry_None, eRtfLanguage_Kanuri);
		ADD_LOCALE(2144, eRtfCountry_None, eRtfLanguage_KashmiriLatin);
		ADD_LOCALE(1120, eRtfCountry_None, eRtfLanguage_KashmiriArabic);
		ADD_LOCALE(1087, eRtfCountry_Kazakhstan, eRtfLanguage_Kazakh);
		ADD_LOCALE(1107, eRtfCountry_Cambodia, eRtfLanguage_Khmer);
		ADD_LOCALE(1158, eRtfCountry_Guatemala, eRtfLanguage_Kiche);
		ADD_LOCALE(1159, eRtfCountry_Rwanda, eRtfLanguage_Kinyarwanda);
		ADD_LOCALE(1089, eRtfCountry_Kenya, eRtfLanguage_Kiswahili);
		ADD_LOCALE(1111, eRtfCountry_India, eRtfLanguage_Konkani);
		ADD_LOCALE(1042, eRtfCountry_Korea, eRtfLanguage_Korean);
		ADD_LOCALE(1088, eRtfCountry_Kyrgyzstan, eRtfLanguage_Kyrgyz);
		ADD_LOCALE(1108, eRtfCountry_LaoPeoplesDemocraticRepublicOf, eRtfLanguage_Lao);
		ADD_LOCALE(1142, eRtfCountry_None, eRtfLanguage_Latin);
		ADD_LOCALE(1062, eRtfCountry_Latvia, eRtfLanguage_Latvian);
		ADD_LOCALE(1063, eRtfCountry_Lithuania, eRtfLanguage_Lithuanian);
		ADD_LOCALE(2087, eRtfCountry_None, eRtfLanguage_LithuanianTrad);
		ADD_LOCALE(2094, eRtfCountry_Germany, eRtfLanguage_LowerSorbian);
		ADD_LOCALE(1071, eRtfCountry_Macedonia, eRtfLanguage_Macedonian);
		ADD_LOCALE(2110, eRtfCountry_BruneiDarussalam, eRtfLanguage_Malay);
		ADD_LOCALE(1086, eRtfCountry_Malaysia, eRtfLanguage_Malay);
		ADD_LOCALE(1100, eRtfCountry_India, eRtfLanguage_Malayalam);
		ADD_LOCALE(1125, eRtfCountry_None, eRtfLanguage_Maldivian);
		ADD_LOCALE(1082, eRtfCountry_Malta, eRtfLanguage_Maltese);
		ADD_LOCALE(1112, eRtfCountry_None, eRtfLanguage_Manipuri);
		ADD_LOCALE(1153, eRtfCountry_NewZealand, eRtfLanguage_Maori);
		ADD_LOCALE(1146, eRtfCountry_Chile, eRtfLanguage_Mapudungun);
		ADD_LOCALE(1102, eRtfCountry_India, eRtfLanguage_Marathi);
		ADD_LOCALE(1148, eRtfCountry_Mohawk, eRtfLanguage_Mohawk);
		ADD_LOCALE(1104, eRtfCountry_Mongolia, eRtfLanguage_MonogolianCyrillic);
		ADD_LOCALE(2128, eRtfCountry_ChinaPeoplesRepublicOf, eRtfLanguage_MonogolianTraditional);
		ADD_LOCALE(2145, eRtfCountry_India, eRtfLanguage_Nepali);
		ADD_LOCALE(0, eRtfCountry_None, eRtfLanguage_None);
		ADD_LOCALE(1024, eRtfCountry_None, eRtfLanguage_None);
		ADD_LOCALE(1044, eRtfCountry_Norway, eRtfLanguage_NorwegianBokmal);
		ADD_LOCALE(2068, eRtfCountry_Norway, eRtfLanguage_NorwegianNynorsk);
		ADD_LOCALE(1154, eRtfCountry_France, eRtfLanguage_Occitan);
		ADD_LOCALE(1096, eRtfCountry_India, eRtfLanguage_Oriya);
		ADD_LOCALE(1138, eRtfCountry_None, eRtfLanguage_Oromo);
		ADD_LOCALE(1145, eRtfCountry_None, eRtfLanguage_Papiamentu);
		ADD_LOCALE(1123, eRtfCountry_Afghanistan, eRtfLanguage_Pashto);
		ADD_LOCALE(1065, eRtfCountry_None, eRtfLanguage_Persian);
		ADD_LOCALE(1045, eRtfCountry_Poland, eRtfLanguage_Polish);
		ADD_LOCALE(1046, eRtfCountry_Brazil, eRtfLanguage_Portuguese);
		ADD_LOCALE(2070, eRtfCountry_Portugal, eRtfLanguage_Portuguese);
		ADD_LOCALE(1094, eRtfCountry_India, eRtfLanguage_Punjabi);
		ADD_LOCALE(2118, eRtfCountry_Pakistan, eRtfLanguage_Punjabi);
		ADD_LOCALE(1131, eRtfCountry_Bolivia, eRtfLanguage_Quechua);
		ADD_LOCALE(2155, eRtfCountry_Ecuador, eRtfLanguage_Quechua);
		ADD_LOCALE(3179, eRtfCountry_Peru, eRtfLanguage_Quechua);
		ADD_LOCALE(2072, eRtfCountry_Moldova, eRtfLanguage_Romanian);
		ADD_LOCALE(1048, eRtfCountry_Romania, eRtfLanguage_Romanian);
		ADD_LOCALE(1047, eRtfCountry_Switzerland, eRtfLanguage_Romansh);
		ADD_LOCALE(2073, eRtfCountry_Moldova, eRtfLanguage_Russian);
		ADD_LOCALE(1049, eRtfCountry_Russia, eRtfLanguage_Russian);
		ADD_LOCALE(9275, eRtfCountry_Finland, eRtfLanguage_SamiInari);
		ADD_LOCALE(4155, eRtfCountry_Norway, eRtfLanguage_SamiLule);
		ADD_LOCALE(5179, eRtfCountry_Sweden, eRtfLanguage_SamiLule);
		ADD_LOCALE(3131, eRtfCountry_Finland, eRtfLanguage_SamiNorthern);
		ADD_LOCALE(1083, eRtfCountry_Norway, eRtfLanguage_SamiNorthern);
		ADD_LOCALE(2107, eRtfCountry_Sweden, eRtfLanguage_SamiNorthern);
		ADD_LOCALE(8251, eRtfCountry_Finland, eRtfLanguage_SamiSkolt);
		ADD_LOCALE(6203, eRtfCountry_Norway, eRtfLanguage_SamiSouthern);
		ADD_LOCALE(7227, eRtfCountry_Sweden, eRtfLanguage_SamiSouthern);
		ADD_LOCALE(1103, eRtfCountry_India, eRtfLanguage_Sanskrit);
		ADD_LOCALE(7194, eRtfCountry_BosniaAndHerzegovina, eRtfLanguage_SerbianCyrillic);
		ADD_LOCALE(3098, eRtfCountry_Serbia, eRtfLanguage_SerbianCyrillic);
		ADD_LOCALE(6170, eRtfCountry_BosniaAndHerzegovina, eRtfLanguage_SerbianLatin);
		ADD_LOCALE(2074, eRtfCountry_Serbia, eRtfLanguage_SerbianLatin);
		ADD_LOCALE(1132, eRtfCountry_SouthAfrica, eRtfLanguage_SesothoSaLeboa);
		ADD_LOCALE(1074, eRtfCountry_SouthAfrica, eRtfLanguage_Setswana);
		ADD_LOCALE(2137, eRtfCountry_None, eRtfLanguage_SindhiArabic);
		ADD_LOCALE(1113, eRtfCountry_None, eRtfLanguage_SindhiDevanagari);
		ADD_LOCALE(1115, eRtfCountry_SriLanka, eRtfLanguage_Sinhala);
		ADD_LOCALE(1051, eRtfCountry_Slovakia, eRtfLanguage_Slovak);
		ADD_LOCALE(1060, eRtfCountry_Slovenia, eRtfLanguage_Slovenian);
		ADD_LOCALE(1143, eRtfCountry_None, eRtfLanguage_Somali);
		ADD_LOCALE(11274, eRtfCountry_Argentina, eRtfLanguage_Spanish);
		ADD_LOCALE(16394, eRtfCountry_Bolivia, eRtfLanguage_Spanish);
		ADD_LOCALE(13322, eRtfCountry_Chile, eRtfLanguage_Spanish);
		ADD_LOCALE(9226, eRtfCountry_Colombia, eRtfLanguage_Spanish);
		ADD_LOCALE(5130, eRtfCountry_CostaRica, eRtfLanguage_Spanish);
		ADD_LOCALE(7178, eRtfCountry_DominicanRepublic, eRtfLanguage_Spanish);
		ADD_LOCALE(12298, eRtfCountry_Ecuador, eRtfLanguage_Spanish);
		ADD_LOCALE(17418, eRtfCountry_ElSalvador, eRtfLanguage_Spanish);
		ADD_LOCALE(4106, eRtfCountry_Guatemala, eRtfLanguage_Spanish);
		ADD_LOCALE(18442, eRtfCountry_Honduras, eRtfLanguage_Spanish);
		ADD_LOCALE(2058, eRtfCountry_Mexico, eRtfLanguage_Spanish);
		ADD_LOCALE(19466, eRtfCountry_Nicaragua, eRtfLanguage_Spanish);
		ADD_LOCALE(6154, eRtfCountry_Panama, eRtfLanguage_Spanish);
		ADD_LOCALE(15370, eRtfCountry_Paraguay, eRtfLanguage_Spanish);
		ADD_LOCALE(10250, eRtfCountry_Peru, eRtfLanguage_Spanish);
		ADD_LOCALE(20490, eRtfCountry_PuertoRico, eRtfLanguage_Spanish);
		ADD_LOCALE(3082, eRtfCountry_Spain, eRtfLanguage_SpanishInternational);
		ADD_LOCALE(1034, eRtfCountry_Spain, eRtfLanguage_SpanishTraditional);
		ADD_LOCALE(21514, eRtfCountry_UnitedStates, eRtfLanguage_Spanish);
		ADD_LOCALE(14346, eRtfCountry_Uruguay, eRtfLanguage_Spanish);
		ADD_LOCALE(8202, eRtfCountry_Venezuela, eRtfLanguage_Spanish);
		ADD_LOCALE(1072, eRtfCountry_SouthAfrica, eRtfLanguage_Sutu);
		ADD_LOCALE(2077, eRtfCountry_Finland, eRtfLanguage_Swedish);
		ADD_LOCALE(1053, eRtfCountry_Sweden, eRtfLanguage_Swedish);
		ADD_LOCALE(1114, eRtfCountry_Syria, eRtfLanguage_Syriac);
		ADD_LOCALE(1064, eRtfCountry_Tajik, eRtfLanguage_TajikCyrillic);
		ADD_LOCALE(1119, eRtfCountry_Morocco, eRtfLanguage_TamazightArabic);
		ADD_LOCALE(2143, eRtfCountry_Algeria, eRtfLanguage_TamazightLatin);
		ADD_LOCALE(1097, eRtfCountry_India, eRtfLanguage_Tamil);
		ADD_LOCALE(1092, eRtfCountry_Russia, eRtfLanguage_Tatar);
		ADD_LOCALE(1098, eRtfCountry_India, eRtfLanguage_Telugu);
		ADD_LOCALE(1054, eRtfCountry_Thailand, eRtfLanguage_Thai);
		ADD_LOCALE(1105, eRtfCountry_ChinaPeoplesRepublicOf, eRtfLanguage_Tibetan);
		ADD_LOCALE(2163, eRtfCountry_Eritrea, eRtfLanguage_Tigrigna);
		ADD_LOCALE(1139, eRtfCountry_Ethiopia, eRtfLanguage_Tigrigna);
		ADD_LOCALE(1073, eRtfCountry_None, eRtfLanguage_Tsonga);
		ADD_LOCALE(1055, eRtfCountry_Turkey, eRtfLanguage_Turkish);
		ADD_LOCALE(1090, eRtfCountry_Turkmenistan, eRtfLanguage_Turkmen);
		ADD_LOCALE(1152, eRtfCountry_ChinaPeoplesRepublicOf, eRtfLanguage_Uighur);
		ADD_LOCALE(1058, eRtfCountry_Ukraine, eRtfLanguage_Ukrainian);
		ADD_LOCALE(1070, eRtfCountry_Germany, eRtfLanguage_UpperSorbian);
		ADD_LOCALE(1056, eRtfCountry_Pakistan, eRtfLanguage_Urdu);
		ADD_LOCALE(2080, eRtfCountry_India, eRtfLanguage_Urdu);
		ADD_LOCALE(2115, eRtfCountry_Uzbekistan, eRtfLanguage_UzbekCyrillic);
		ADD_LOCALE(1091, eRtfCountry_Uzbekistan, eRtfLanguage_UzbekLatin);
		ADD_LOCALE(1075, eRtfCountry_None, eRtfLanguage_Venda);
		ADD_LOCALE(1066, eRtfCountry_Vietnam, eRtfLanguage_Vietnamese);
		ADD_LOCALE(1106, eRtfCountry_UnitedKingdom, eRtfLanguage_Welsh);
		ADD_LOCALE(1160, eRtfCountry_Senegal, eRtfLanguage_Wolof);
		ADD_LOCALE(1076, eRtfCountry_None, eRtfLanguage_Xhosa);
		ADD_LOCALE(1157, eRtfCountry_Russia, eRtfLanguage_Yakut);
		ADD_LOCALE(1144, eRtfCountry_ChinaPeoplesRepublicOf, eRtfLanguage_Yi);
		ADD_LOCALE(1085, eRtfCountry_None, eRtfLanguage_Yiddish);
		ADD_LOCALE(1130, eRtfCountry_Nigeria, eRtfLanguage_Yoruba);

#undef ADD_LOCALE
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