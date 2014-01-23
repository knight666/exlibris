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
#define ADD_LOCALE(_id, _country, _language) m_Locales.insert(std::make_pair(_id, RtfLocale(_id, _country, _language)));

		ADD_LOCALE(1078, Rtf::eCountry_SouthAfrica, eRtfLanguage_Afrikaans);
		ADD_LOCALE(1052, Rtf::eCountry_Albania, eRtfLanguage_Albanian);
		ADD_LOCALE(1156, Rtf::eCountry_France, eRtfLanguage_Alsatian);
		ADD_LOCALE(1118, Rtf::eCountry_Ethiopia, eRtfLanguage_Amharic);
		ADD_LOCALE(5121, Rtf::eCountry_Algeria, eRtfLanguage_Arabic);
		ADD_LOCALE(15361, Rtf::eCountry_Bahrain, eRtfLanguage_Arabic);
		ADD_LOCALE(3073, Rtf::eCountry_Egypt, eRtfLanguage_Arabic);
		ADD_LOCALE(2049, Rtf::eCountry_Iraq, eRtfLanguage_Arabic);
		ADD_LOCALE(11265, Rtf::eCountry_Jordan, eRtfLanguage_Arabic);
		ADD_LOCALE(13313, Rtf::eCountry_Kuwait, eRtfLanguage_Arabic);
		ADD_LOCALE(12289, Rtf::eCountry_Lebanon, eRtfLanguage_Arabic);
		ADD_LOCALE(4097, Rtf::eCountry_Libya, eRtfLanguage_Arabic);
		ADD_LOCALE(6145, Rtf::eCountry_Morocco, eRtfLanguage_Arabic);
		ADD_LOCALE(8193, Rtf::eCountry_Oman, eRtfLanguage_Arabic);
		ADD_LOCALE(16385, Rtf::eCountry_Qatar, eRtfLanguage_Arabic);
		ADD_LOCALE(1025, Rtf::eCountry_SaudiArabia, eRtfLanguage_Arabic);
		ADD_LOCALE(10241, Rtf::eCountry_Syria, eRtfLanguage_Arabic);
		ADD_LOCALE(7169, Rtf::eCountry_Tunisia, eRtfLanguage_Arabic);
		ADD_LOCALE(14337, Rtf::eCountry_UnitedArabicEmirates, eRtfLanguage_Arabic);
		ADD_LOCALE(9217, Rtf::eCountry_Yemen, eRtfLanguage_Arabic);
		ADD_LOCALE(1067, Rtf::eCountry_Armenia, eRtfLanguage_Armenian);
		ADD_LOCALE(1101, Rtf::eCountry_India, eRtfLanguage_Assamese);
		ADD_LOCALE(2092, Rtf::eCountry_Azerbaijan, eRtfLanguage_AzeriCyrillic);
		ADD_LOCALE(1068, Rtf::eCountry_Azerbaijan, eRtfLanguage_AzeriLatin);
		ADD_LOCALE(1133, Rtf::eCountry_Russia, eRtfLanguage_Bashkir);
		ADD_LOCALE(1069, Rtf::eCountry_Basque, eRtfLanguage_Basque);
		ADD_LOCALE(1059, Rtf::eCountry_Belarus, eRtfLanguage_Belarusian);
		ADD_LOCALE(2117, Rtf::eCountry_Bangladesh, eRtfLanguage_Bengali);
		ADD_LOCALE(1093, Rtf::eCountry_India, eRtfLanguage_Bengali);
		ADD_LOCALE(4122, Rtf::eCountry_None, eRtfLanguage_BosnianLatin);
		ADD_LOCALE(8218, Rtf::eCountry_BosniaAndHerzegovina, eRtfLanguage_BosnianCyrillic);
		ADD_LOCALE(5146, Rtf::eCountry_BosniaAndHerzegovina, eRtfLanguage_BosnianLatin);
		ADD_LOCALE(1150, Rtf::eCountry_France, eRtfLanguage_Breton);
		ADD_LOCALE(1026, Rtf::eCountry_Bulgaria, eRtfLanguage_Bulgarian);
		ADD_LOCALE(1109, Rtf::eCountry_None, eRtfLanguage_Burmese);
		ADD_LOCALE(1027, Rtf::eCountry_Catalan, eRtfLanguage_Catalan);
		ADD_LOCALE(1116, Rtf::eCountry_None, eRtfLanguage_Cherokee);
		ADD_LOCALE(3076, Rtf::eCountry_SpecialAdministrativeRegionHongKong, eRtfLanguage_Chinese);
		ADD_LOCALE(5124, Rtf::eCountry_SpecialAdministrativeRegionMacao, eRtfLanguage_Chinese);
		ADD_LOCALE(2052, Rtf::eCountry_ChinaPeoplesRepublicOf, eRtfLanguage_Chinese);
		ADD_LOCALE(4100, Rtf::eCountry_Singapore, eRtfLanguage_Chinese);
		ADD_LOCALE(1028, Rtf::eCountry_Taiwan, eRtfLanguage_Chinese);
		ADD_LOCALE(1155, Rtf::eCountry_France, eRtfLanguage_Corsican);
		ADD_LOCALE(1050, Rtf::eCountry_Croatia, eRtfLanguage_Croatian);
		ADD_LOCALE(4122, Rtf::eCountry_BosniaAndHerzegovina, eRtfLanguage_CroatianLatin);
		ADD_LOCALE(3072, Rtf::eCountry_Current, eRtfLanguage_Custom);
		ADD_LOCALE(1029, Rtf::eCountry_CzechRepublic, eRtfLanguage_Czech);
		ADD_LOCALE(1030, Rtf::eCountry_Denmark, eRtfLanguage_Danish);
		ADD_LOCALE(1164, Rtf::eCountry_Afghanistan, eRtfLanguage_Dari);
		ADD_LOCALE(1125, Rtf::eCountry_Maldives, eRtfLanguage_Divehi);
		ADD_LOCALE(2067, Rtf::eCountry_Belgium, eRtfLanguage_Dutch);
		ADD_LOCALE(1043, Rtf::eCountry_Netherlands, eRtfLanguage_Dutch);
		ADD_LOCALE(19, Rtf::eCountry_Preferred, eRtfLanguage_Dutch);
		ADD_LOCALE(1126, Rtf::eCountry_None, eRtfLanguage_Dzongkha);
		ADD_LOCALE(3081, Rtf::eCountry_Australia, eRtfLanguage_English);
		ADD_LOCALE(10249, Rtf::eCountry_Belize, eRtfLanguage_English);
		ADD_LOCALE(4105, Rtf::eCountry_Canada, eRtfLanguage_English);
		ADD_LOCALE(9225, Rtf::eCountry_Caribbean, eRtfLanguage_English);
		ADD_LOCALE(15369, Rtf::eCountry_SpecialAdministrativeRegionHongKong, eRtfLanguage_English);
		ADD_LOCALE(16393, Rtf::eCountry_India, eRtfLanguage_English);
		ADD_LOCALE(14345, Rtf::eCountry_Indonesia, eRtfLanguage_English);
		ADD_LOCALE(6153, Rtf::eCountry_Ireland, eRtfLanguage_English);
		ADD_LOCALE(8201, Rtf::eCountry_Jamaica, eRtfLanguage_English);
		ADD_LOCALE(17417, Rtf::eCountry_Malaysia, eRtfLanguage_English);
		ADD_LOCALE(5129, Rtf::eCountry_NewZealand, eRtfLanguage_English);
		ADD_LOCALE(13321, Rtf::eCountry_Philippines, eRtfLanguage_English);
		ADD_LOCALE(18441, Rtf::eCountry_Singapore, eRtfLanguage_English);
		ADD_LOCALE(7177, Rtf::eCountry_SouthAfrica, eRtfLanguage_English);
		ADD_LOCALE(11273, Rtf::eCountry_TrinidadAndTobago, eRtfLanguage_English);
		ADD_LOCALE(2057, Rtf::eCountry_UnitedKingdom, eRtfLanguage_English);
		ADD_LOCALE(1033, Rtf::eCountry_UnitedStates, eRtfLanguage_English);
		ADD_LOCALE(11297, Rtf::eCountry_Zimbabwe, eRtfLanguage_English);
		ADD_LOCALE(1061, Rtf::eCountry_Estonia, eRtfLanguage_Estonian);
		ADD_LOCALE(1080, Rtf::eCountry_FaroeIslands, eRtfLanguage_Faroese);
		ADD_LOCALE(1124, Rtf::eCountry_Philippines, eRtfLanguage_Filipino);
		ADD_LOCALE(1035, Rtf::eCountry_Finland, eRtfLanguage_Finnish);
		ADD_LOCALE(2060, Rtf::eCountry_Belgium, eRtfLanguage_French);
		ADD_LOCALE(11276, Rtf::eCountry_Cameroon, eRtfLanguage_French);
		ADD_LOCALE(3084, Rtf::eCountry_Canada, eRtfLanguage_French);
		ADD_LOCALE(9228, Rtf::eCountry_CongoDemocraticRepublicOf, eRtfLanguage_French);
		ADD_LOCALE(12300, Rtf::eCountry_CotedIvoire, eRtfLanguage_French);
		ADD_LOCALE(1036, Rtf::eCountry_France, eRtfLanguage_French);
		ADD_LOCALE(15372, Rtf::eCountry_Haiti, eRtfLanguage_French);
		ADD_LOCALE(5132, Rtf::eCountry_Luxembourg, eRtfLanguage_French);
		ADD_LOCALE(13324, Rtf::eCountry_Mali, eRtfLanguage_French);
		ADD_LOCALE(6156, Rtf::eCountry_Monaco, eRtfLanguage_French);
		ADD_LOCALE(14348, Rtf::eCountry_Morocco, eRtfLanguage_French);
		ADD_LOCALE(8204, Rtf::eCountry_Reunion, eRtfLanguage_French);
		ADD_LOCALE(10252, Rtf::eCountry_Senegal, eRtfLanguage_French);
		ADD_LOCALE(4108, Rtf::eCountry_Switzerland, eRtfLanguage_French);
		ADD_LOCALE(7180, Rtf::eCountry_WestIndies, eRtfLanguage_French);
		ADD_LOCALE(1132, Rtf::eCountry_Netherlands, eRtfLanguage_Frisian);
		ADD_LOCALE(1127, Rtf::eCountry_None, eRtfLanguage_Fulfukle);
		ADD_LOCALE(2108, Rtf::eCountry_Ireland, eRtfLanguage_Gaelic);
		ADD_LOCALE(1084, Rtf::eCountry_Scotland, eRtfLanguage_Gaelic);
		ADD_LOCALE(1079, Rtf::eCountry_Georgia, eRtfLanguage_Georgian);
		ADD_LOCALE(3079, Rtf::eCountry_Austria, eRtfLanguage_German);
		ADD_LOCALE(1031, Rtf::eCountry_Germany, eRtfLanguage_German);
		ADD_LOCALE(5127, Rtf::eCountry_Liechtenstein, eRtfLanguage_German);
		ADD_LOCALE(4103, Rtf::eCountry_Luxembourg, eRtfLanguage_German);
		ADD_LOCALE(2055, Rtf::eCountry_Switzerland, eRtfLanguage_German);
		ADD_LOCALE(1032, Rtf::eCountry_Greece, eRtfLanguage_Greek);
		ADD_LOCALE(1135, Rtf::eCountry_Greenland, eRtfLanguage_Greenlandic);
		ADD_LOCALE(1140, Rtf::eCountry_None, eRtfLanguage_Guarani);
		ADD_LOCALE(1095, Rtf::eCountry_India, eRtfLanguage_Gujarati);
		ADD_LOCALE(1128, Rtf::eCountry_Nigeria, eRtfLanguage_HausaLatin);
		ADD_LOCALE(1141, Rtf::eCountry_None, eRtfLanguage_Hawaiian);
		ADD_LOCALE(1037, Rtf::eCountry_Israel, eRtfLanguage_Hebrew);
		ADD_LOCALE(1081, Rtf::eCountry_India, eRtfLanguage_Hindi);
		ADD_LOCALE(1038, Rtf::eCountry_Hungary, eRtfLanguage_Hungarian);
		ADD_LOCALE(1129, Rtf::eCountry_Nigeria, eRtfLanguage_Ibibio);
		ADD_LOCALE(1039, Rtf::eCountry_Iceland, eRtfLanguage_Icelandic);
		ADD_LOCALE(1136, Rtf::eCountry_Nigeria, eRtfLanguage_Igbo);
		ADD_LOCALE(2141, Rtf::eCountry_Canada, eRtfLanguage_InuktitutLatin);
		ADD_LOCALE(1117, Rtf::eCountry_Canada, eRtfLanguage_InuktitutSyllabics);
		ADD_LOCALE(1076, Rtf::eCountry_SouthAfrica, eRtfLanguage_IsiXhosa);
		ADD_LOCALE(1077, Rtf::eCountry_SouthAfrica, eRtfLanguage_IsiZulu);
		ADD_LOCALE(1040, Rtf::eCountry_Italy, eRtfLanguage_Italian);
		ADD_LOCALE(2064, Rtf::eCountry_Switzerland, eRtfLanguage_Italian);
		ADD_LOCALE(1041, Rtf::eCountry_Japan, eRtfLanguage_Japanese);
		ADD_LOCALE(1099, Rtf::eCountry_India, eRtfLanguage_Kannada);
		ADD_LOCALE(1137, Rtf::eCountry_None, eRtfLanguage_Kanuri);
		ADD_LOCALE(2144, Rtf::eCountry_None, eRtfLanguage_KashmiriLatin);
		ADD_LOCALE(1120, Rtf::eCountry_None, eRtfLanguage_KashmiriArabic);
		ADD_LOCALE(1087, Rtf::eCountry_Kazakhstan, eRtfLanguage_Kazakh);
		ADD_LOCALE(1107, Rtf::eCountry_Cambodia, eRtfLanguage_Khmer);
		ADD_LOCALE(1158, Rtf::eCountry_Guatemala, eRtfLanguage_Kiche);
		ADD_LOCALE(1159, Rtf::eCountry_Rwanda, eRtfLanguage_Kinyarwanda);
		ADD_LOCALE(1089, Rtf::eCountry_Kenya, eRtfLanguage_Kiswahili);
		ADD_LOCALE(1111, Rtf::eCountry_India, eRtfLanguage_Konkani);
		ADD_LOCALE(1042, Rtf::eCountry_Korea, eRtfLanguage_Korean);
		ADD_LOCALE(1088, Rtf::eCountry_Kyrgyzstan, eRtfLanguage_Kyrgyz);
		ADD_LOCALE(1108, Rtf::eCountry_LaoPeoplesDemocraticRepublicOf, eRtfLanguage_Lao);
		ADD_LOCALE(1142, Rtf::eCountry_None, eRtfLanguage_Latin);
		ADD_LOCALE(1062, Rtf::eCountry_Latvia, eRtfLanguage_Latvian);
		ADD_LOCALE(1063, Rtf::eCountry_Lithuania, eRtfLanguage_Lithuanian);
		ADD_LOCALE(2087, Rtf::eCountry_None, eRtfLanguage_LithuanianTrad);
		ADD_LOCALE(2094, Rtf::eCountry_Germany, eRtfLanguage_LowerSorbian);
		ADD_LOCALE(1071, Rtf::eCountry_Macedonia, eRtfLanguage_Macedonian);
		ADD_LOCALE(2110, Rtf::eCountry_BruneiDarussalam, eRtfLanguage_Malay);
		ADD_LOCALE(1086, Rtf::eCountry_Malaysia, eRtfLanguage_Malay);
		ADD_LOCALE(1100, Rtf::eCountry_India, eRtfLanguage_Malayalam);
		ADD_LOCALE(1125, Rtf::eCountry_None, eRtfLanguage_Maldivian);
		ADD_LOCALE(1082, Rtf::eCountry_Malta, eRtfLanguage_Maltese);
		ADD_LOCALE(1112, Rtf::eCountry_None, eRtfLanguage_Manipuri);
		ADD_LOCALE(1153, Rtf::eCountry_NewZealand, eRtfLanguage_Maori);
		ADD_LOCALE(1146, Rtf::eCountry_Chile, eRtfLanguage_Mapudungun);
		ADD_LOCALE(1102, Rtf::eCountry_India, eRtfLanguage_Marathi);
		ADD_LOCALE(1148, Rtf::eCountry_Mohawk, eRtfLanguage_Mohawk);
		ADD_LOCALE(1104, Rtf::eCountry_Mongolia, eRtfLanguage_MonogolianCyrillic);
		ADD_LOCALE(2128, Rtf::eCountry_ChinaPeoplesRepublicOf, eRtfLanguage_MonogolianTraditional);
		ADD_LOCALE(2145, Rtf::eCountry_India, eRtfLanguage_Nepali);
		ADD_LOCALE(0, Rtf::eCountry_None, eRtfLanguage_None);
		ADD_LOCALE(1024, Rtf::eCountry_None, eRtfLanguage_None);
		ADD_LOCALE(1044, Rtf::eCountry_Norway, eRtfLanguage_NorwegianBokmal);
		ADD_LOCALE(2068, Rtf::eCountry_Norway, eRtfLanguage_NorwegianNynorsk);
		ADD_LOCALE(1154, Rtf::eCountry_France, eRtfLanguage_Occitan);
		ADD_LOCALE(1096, Rtf::eCountry_India, eRtfLanguage_Oriya);
		ADD_LOCALE(1138, Rtf::eCountry_None, eRtfLanguage_Oromo);
		ADD_LOCALE(1145, Rtf::eCountry_None, eRtfLanguage_Papiamentu);
		ADD_LOCALE(1123, Rtf::eCountry_Afghanistan, eRtfLanguage_Pashto);
		ADD_LOCALE(1065, Rtf::eCountry_None, eRtfLanguage_Persian);
		ADD_LOCALE(1045, Rtf::eCountry_Poland, eRtfLanguage_Polish);
		ADD_LOCALE(1046, Rtf::eCountry_Brazil, eRtfLanguage_Portuguese);
		ADD_LOCALE(2070, Rtf::eCountry_Portugal, eRtfLanguage_Portuguese);
		ADD_LOCALE(1094, Rtf::eCountry_India, eRtfLanguage_Punjabi);
		ADD_LOCALE(2118, Rtf::eCountry_Pakistan, eRtfLanguage_Punjabi);
		ADD_LOCALE(1131, Rtf::eCountry_Bolivia, eRtfLanguage_Quechua);
		ADD_LOCALE(2155, Rtf::eCountry_Ecuador, eRtfLanguage_Quechua);
		ADD_LOCALE(3179, Rtf::eCountry_Peru, eRtfLanguage_Quechua);
		ADD_LOCALE(2072, Rtf::eCountry_Moldova, eRtfLanguage_Romanian);
		ADD_LOCALE(1048, Rtf::eCountry_Romania, eRtfLanguage_Romanian);
		ADD_LOCALE(1047, Rtf::eCountry_Switzerland, eRtfLanguage_Romansh);
		ADD_LOCALE(2073, Rtf::eCountry_Moldova, eRtfLanguage_Russian);
		ADD_LOCALE(1049, Rtf::eCountry_Russia, eRtfLanguage_Russian);
		ADD_LOCALE(9275, Rtf::eCountry_Finland, eRtfLanguage_SamiInari);
		ADD_LOCALE(4155, Rtf::eCountry_Norway, eRtfLanguage_SamiLule);
		ADD_LOCALE(5179, Rtf::eCountry_Sweden, eRtfLanguage_SamiLule);
		ADD_LOCALE(3131, Rtf::eCountry_Finland, eRtfLanguage_SamiNorthern);
		ADD_LOCALE(1083, Rtf::eCountry_Norway, eRtfLanguage_SamiNorthern);
		ADD_LOCALE(2107, Rtf::eCountry_Sweden, eRtfLanguage_SamiNorthern);
		ADD_LOCALE(8251, Rtf::eCountry_Finland, eRtfLanguage_SamiSkolt);
		ADD_LOCALE(6203, Rtf::eCountry_Norway, eRtfLanguage_SamiSouthern);
		ADD_LOCALE(7227, Rtf::eCountry_Sweden, eRtfLanguage_SamiSouthern);
		ADD_LOCALE(1103, Rtf::eCountry_India, eRtfLanguage_Sanskrit);
		ADD_LOCALE(7194, Rtf::eCountry_BosniaAndHerzegovina, eRtfLanguage_SerbianCyrillic);
		ADD_LOCALE(3098, Rtf::eCountry_Serbia, eRtfLanguage_SerbianCyrillic);
		ADD_LOCALE(6170, Rtf::eCountry_BosniaAndHerzegovina, eRtfLanguage_SerbianLatin);
		ADD_LOCALE(2074, Rtf::eCountry_Serbia, eRtfLanguage_SerbianLatin);
		ADD_LOCALE(1132, Rtf::eCountry_SouthAfrica, eRtfLanguage_SesothoSaLeboa);
		ADD_LOCALE(1074, Rtf::eCountry_SouthAfrica, eRtfLanguage_Setswana);
		ADD_LOCALE(2137, Rtf::eCountry_None, eRtfLanguage_SindhiArabic);
		ADD_LOCALE(1113, Rtf::eCountry_None, eRtfLanguage_SindhiDevanagari);
		ADD_LOCALE(1115, Rtf::eCountry_SriLanka, eRtfLanguage_Sinhala);
		ADD_LOCALE(1051, Rtf::eCountry_Slovakia, eRtfLanguage_Slovak);
		ADD_LOCALE(1060, Rtf::eCountry_Slovenia, eRtfLanguage_Slovenian);
		ADD_LOCALE(1143, Rtf::eCountry_None, eRtfLanguage_Somali);
		ADD_LOCALE(11274, Rtf::eCountry_Argentina, eRtfLanguage_Spanish);
		ADD_LOCALE(16394, Rtf::eCountry_Bolivia, eRtfLanguage_Spanish);
		ADD_LOCALE(13322, Rtf::eCountry_Chile, eRtfLanguage_Spanish);
		ADD_LOCALE(9226, Rtf::eCountry_Colombia, eRtfLanguage_Spanish);
		ADD_LOCALE(5130, Rtf::eCountry_CostaRica, eRtfLanguage_Spanish);
		ADD_LOCALE(7178, Rtf::eCountry_DominicanRepublic, eRtfLanguage_Spanish);
		ADD_LOCALE(12298, Rtf::eCountry_Ecuador, eRtfLanguage_Spanish);
		ADD_LOCALE(17418, Rtf::eCountry_ElSalvador, eRtfLanguage_Spanish);
		ADD_LOCALE(4106, Rtf::eCountry_Guatemala, eRtfLanguage_Spanish);
		ADD_LOCALE(18442, Rtf::eCountry_Honduras, eRtfLanguage_Spanish);
		ADD_LOCALE(2058, Rtf::eCountry_Mexico, eRtfLanguage_Spanish);
		ADD_LOCALE(19466, Rtf::eCountry_Nicaragua, eRtfLanguage_Spanish);
		ADD_LOCALE(6154, Rtf::eCountry_Panama, eRtfLanguage_Spanish);
		ADD_LOCALE(15370, Rtf::eCountry_Paraguay, eRtfLanguage_Spanish);
		ADD_LOCALE(10250, Rtf::eCountry_Peru, eRtfLanguage_Spanish);
		ADD_LOCALE(20490, Rtf::eCountry_PuertoRico, eRtfLanguage_Spanish);
		ADD_LOCALE(3082, Rtf::eCountry_Spain, eRtfLanguage_SpanishInternational);
		ADD_LOCALE(1034, Rtf::eCountry_Spain, eRtfLanguage_SpanishTraditional);
		ADD_LOCALE(21514, Rtf::eCountry_UnitedStates, eRtfLanguage_Spanish);
		ADD_LOCALE(14346, Rtf::eCountry_Uruguay, eRtfLanguage_Spanish);
		ADD_LOCALE(8202, Rtf::eCountry_Venezuela, eRtfLanguage_Spanish);
		ADD_LOCALE(1072, Rtf::eCountry_SouthAfrica, eRtfLanguage_Sutu);
		ADD_LOCALE(2077, Rtf::eCountry_Finland, eRtfLanguage_Swedish);
		ADD_LOCALE(1053, Rtf::eCountry_Sweden, eRtfLanguage_Swedish);
		ADD_LOCALE(1114, Rtf::eCountry_Syria, eRtfLanguage_Syriac);
		ADD_LOCALE(1064, Rtf::eCountry_Tajik, eRtfLanguage_TajikCyrillic);
		ADD_LOCALE(1119, Rtf::eCountry_Morocco, eRtfLanguage_TamazightArabic);
		ADD_LOCALE(2143, Rtf::eCountry_Algeria, eRtfLanguage_TamazightLatin);
		ADD_LOCALE(1097, Rtf::eCountry_India, eRtfLanguage_Tamil);
		ADD_LOCALE(1092, Rtf::eCountry_Russia, eRtfLanguage_Tatar);
		ADD_LOCALE(1098, Rtf::eCountry_India, eRtfLanguage_Telugu);
		ADD_LOCALE(1054, Rtf::eCountry_Thailand, eRtfLanguage_Thai);
		ADD_LOCALE(1105, Rtf::eCountry_ChinaPeoplesRepublicOf, eRtfLanguage_Tibetan);
		ADD_LOCALE(2163, Rtf::eCountry_Eritrea, eRtfLanguage_Tigrigna);
		ADD_LOCALE(1139, Rtf::eCountry_Ethiopia, eRtfLanguage_Tigrigna);
		ADD_LOCALE(1073, Rtf::eCountry_None, eRtfLanguage_Tsonga);
		ADD_LOCALE(1055, Rtf::eCountry_Turkey, eRtfLanguage_Turkish);
		ADD_LOCALE(1090, Rtf::eCountry_Turkmenistan, eRtfLanguage_Turkmen);
		ADD_LOCALE(1152, Rtf::eCountry_ChinaPeoplesRepublicOf, eRtfLanguage_Uighur);
		ADD_LOCALE(1058, Rtf::eCountry_Ukraine, eRtfLanguage_Ukrainian);
		ADD_LOCALE(1070, Rtf::eCountry_Germany, eRtfLanguage_UpperSorbian);
		ADD_LOCALE(1056, Rtf::eCountry_Pakistan, eRtfLanguage_Urdu);
		ADD_LOCALE(2080, Rtf::eCountry_India, eRtfLanguage_Urdu);
		ADD_LOCALE(2115, Rtf::eCountry_Uzbekistan, eRtfLanguage_UzbekCyrillic);
		ADD_LOCALE(1091, Rtf::eCountry_Uzbekistan, eRtfLanguage_UzbekLatin);
		ADD_LOCALE(1075, Rtf::eCountry_None, eRtfLanguage_Venda);
		ADD_LOCALE(1066, Rtf::eCountry_Vietnam, eRtfLanguage_Vietnamese);
		ADD_LOCALE(1106, Rtf::eCountry_UnitedKingdom, eRtfLanguage_Welsh);
		ADD_LOCALE(1160, Rtf::eCountry_Senegal, eRtfLanguage_Wolof);
		ADD_LOCALE(1076, Rtf::eCountry_None, eRtfLanguage_Xhosa);
		ADD_LOCALE(1157, Rtf::eCountry_Russia, eRtfLanguage_Yakut);
		ADD_LOCALE(1144, Rtf::eCountry_ChinaPeoplesRepublicOf, eRtfLanguage_Yi);
		ADD_LOCALE(1085, Rtf::eCountry_None, eRtfLanguage_Yiddish);
		ADD_LOCALE(1130, Rtf::eCountry_Nigeria, eRtfLanguage_Yoruba);

#undef ADD_LOCALE
	}

	RtfWorld::~RtfWorld()
	{
		m_Locales.clear();
	}

	const RtfLocale* RtfWorld::GetLocaleByIdentifier(unsigned int a_Index) const
	{
		std::map<unsigned int, RtfLocale>::const_iterator found = m_Locales.find(a_Index);
		if (found != m_Locales.end())
		{
			return &found->second;
		}
		else
		{
			return nullptr;
		}
	}

	unsigned int RtfWorld::GetLocaleIdentifierByLanguageAndCountry(RtfLanguage a_Language, Rtf::Country a_Country) const
	{
		for (std::map<unsigned int, RtfLocale>::const_iterator entry_it = m_Locales.begin(); entry_it != m_Locales.end(); ++entry_it)
		{
			const RtfLocale& entry = entry_it->second;
			if (entry.country == a_Country && entry.language == a_Language)
			{
				return entry.identifier;
			}
		}

		return 0;
	}

	std::set<RtfLanguage> RtfWorld::GetLanguagesForCountry(Rtf::Country a_Country) const
	{
		std::set<RtfLanguage> results;

		for (std::map<unsigned int, RtfLocale>::const_iterator entry_it = m_Locales.begin(); entry_it != m_Locales.end(); ++entry_it)
		{
			const RtfLocale& entry = entry_it->second;
			if (entry.country == a_Country)
			{
				results.insert(entry.language);
			}
		}

		return results;
	}

	std::set<Rtf::Country> RtfWorld::GetCountriesForLanguage(RtfLanguage a_Language) const
	{
		std::set<Rtf::Country> results;

		for (std::map<unsigned int, RtfLocale>::const_iterator entry_it = m_Locales.begin(); entry_it != m_Locales.end(); ++entry_it)
		{
			const RtfLocale& entry = entry_it->second;
			if (entry.language == a_Language)
			{
				results.insert(entry.country);
			}
		}

		return results;
	}

}; // namespace ExLibris