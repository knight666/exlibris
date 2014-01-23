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

		ADD_LOCALE(1078, Rtf::eCountry_SouthAfrica, Rtf::eLanguage_Afrikaans);
		ADD_LOCALE(1052, Rtf::eCountry_Albania, Rtf::eLanguage_Albanian);
		ADD_LOCALE(1156, Rtf::eCountry_France, Rtf::eLanguage_Alsatian);
		ADD_LOCALE(1118, Rtf::eCountry_Ethiopia, Rtf::eLanguage_Amharic);
		ADD_LOCALE(5121, Rtf::eCountry_Algeria, Rtf::eLanguage_Arabic);
		ADD_LOCALE(15361, Rtf::eCountry_Bahrain, Rtf::eLanguage_Arabic);
		ADD_LOCALE(3073, Rtf::eCountry_Egypt, Rtf::eLanguage_Arabic);
		ADD_LOCALE(2049, Rtf::eCountry_Iraq, Rtf::eLanguage_Arabic);
		ADD_LOCALE(11265, Rtf::eCountry_Jordan, Rtf::eLanguage_Arabic);
		ADD_LOCALE(13313, Rtf::eCountry_Kuwait, Rtf::eLanguage_Arabic);
		ADD_LOCALE(12289, Rtf::eCountry_Lebanon, Rtf::eLanguage_Arabic);
		ADD_LOCALE(4097, Rtf::eCountry_Libya, Rtf::eLanguage_Arabic);
		ADD_LOCALE(6145, Rtf::eCountry_Morocco, Rtf::eLanguage_Arabic);
		ADD_LOCALE(8193, Rtf::eCountry_Oman, Rtf::eLanguage_Arabic);
		ADD_LOCALE(16385, Rtf::eCountry_Qatar, Rtf::eLanguage_Arabic);
		ADD_LOCALE(1025, Rtf::eCountry_SaudiArabia, Rtf::eLanguage_Arabic);
		ADD_LOCALE(10241, Rtf::eCountry_Syria, Rtf::eLanguage_Arabic);
		ADD_LOCALE(7169, Rtf::eCountry_Tunisia, Rtf::eLanguage_Arabic);
		ADD_LOCALE(14337, Rtf::eCountry_UnitedArabicEmirates, Rtf::eLanguage_Arabic);
		ADD_LOCALE(9217, Rtf::eCountry_Yemen, Rtf::eLanguage_Arabic);
		ADD_LOCALE(1067, Rtf::eCountry_Armenia, Rtf::eLanguage_Armenian);
		ADD_LOCALE(1101, Rtf::eCountry_India, Rtf::eLanguage_Assamese);
		ADD_LOCALE(2092, Rtf::eCountry_Azerbaijan, Rtf::eLanguage_AzeriCyrillic);
		ADD_LOCALE(1068, Rtf::eCountry_Azerbaijan, Rtf::eLanguage_AzeriLatin);
		ADD_LOCALE(1133, Rtf::eCountry_Russia, Rtf::eLanguage_Bashkir);
		ADD_LOCALE(1069, Rtf::eCountry_Basque, Rtf::eLanguage_Basque);
		ADD_LOCALE(1059, Rtf::eCountry_Belarus, Rtf::eLanguage_Belarusian);
		ADD_LOCALE(2117, Rtf::eCountry_Bangladesh, Rtf::eLanguage_Bengali);
		ADD_LOCALE(1093, Rtf::eCountry_India, Rtf::eLanguage_Bengali);
		ADD_LOCALE(4122, Rtf::eCountry_None, Rtf::eLanguage_BosnianLatin);
		ADD_LOCALE(8218, Rtf::eCountry_BosniaAndHerzegovina, Rtf::eLanguage_BosnianCyrillic);
		ADD_LOCALE(5146, Rtf::eCountry_BosniaAndHerzegovina, Rtf::eLanguage_BosnianLatin);
		ADD_LOCALE(1150, Rtf::eCountry_France, Rtf::eLanguage_Breton);
		ADD_LOCALE(1026, Rtf::eCountry_Bulgaria, Rtf::eLanguage_Bulgarian);
		ADD_LOCALE(1109, Rtf::eCountry_None, Rtf::eLanguage_Burmese);
		ADD_LOCALE(1027, Rtf::eCountry_Catalan, Rtf::eLanguage_Catalan);
		ADD_LOCALE(1116, Rtf::eCountry_None, Rtf::eLanguage_Cherokee);
		ADD_LOCALE(3076, Rtf::eCountry_SpecialAdministrativeRegionHongKong, Rtf::eLanguage_Chinese);
		ADD_LOCALE(5124, Rtf::eCountry_SpecialAdministrativeRegionMacao, Rtf::eLanguage_Chinese);
		ADD_LOCALE(2052, Rtf::eCountry_ChinaPeoplesRepublicOf, Rtf::eLanguage_Chinese);
		ADD_LOCALE(4100, Rtf::eCountry_Singapore, Rtf::eLanguage_Chinese);
		ADD_LOCALE(1028, Rtf::eCountry_Taiwan, Rtf::eLanguage_Chinese);
		ADD_LOCALE(1155, Rtf::eCountry_France, Rtf::eLanguage_Corsican);
		ADD_LOCALE(1050, Rtf::eCountry_Croatia, Rtf::eLanguage_Croatian);
		ADD_LOCALE(4122, Rtf::eCountry_BosniaAndHerzegovina, Rtf::eLanguage_CroatianLatin);
		ADD_LOCALE(3072, Rtf::eCountry_Current, Rtf::eLanguage_Custom);
		ADD_LOCALE(1029, Rtf::eCountry_CzechRepublic, Rtf::eLanguage_Czech);
		ADD_LOCALE(1030, Rtf::eCountry_Denmark, Rtf::eLanguage_Danish);
		ADD_LOCALE(1164, Rtf::eCountry_Afghanistan, Rtf::eLanguage_Dari);
		ADD_LOCALE(1125, Rtf::eCountry_Maldives, Rtf::eLanguage_Divehi);
		ADD_LOCALE(2067, Rtf::eCountry_Belgium, Rtf::eLanguage_Dutch);
		ADD_LOCALE(1043, Rtf::eCountry_Netherlands, Rtf::eLanguage_Dutch);
		ADD_LOCALE(19, Rtf::eCountry_Preferred, Rtf::eLanguage_Dutch);
		ADD_LOCALE(1126, Rtf::eCountry_None, Rtf::eLanguage_Dzongkha);
		ADD_LOCALE(3081, Rtf::eCountry_Australia, Rtf::eLanguage_English);
		ADD_LOCALE(10249, Rtf::eCountry_Belize, Rtf::eLanguage_English);
		ADD_LOCALE(4105, Rtf::eCountry_Canada, Rtf::eLanguage_English);
		ADD_LOCALE(9225, Rtf::eCountry_Caribbean, Rtf::eLanguage_English);
		ADD_LOCALE(15369, Rtf::eCountry_SpecialAdministrativeRegionHongKong, Rtf::eLanguage_English);
		ADD_LOCALE(16393, Rtf::eCountry_India, Rtf::eLanguage_English);
		ADD_LOCALE(14345, Rtf::eCountry_Indonesia, Rtf::eLanguage_English);
		ADD_LOCALE(6153, Rtf::eCountry_Ireland, Rtf::eLanguage_English);
		ADD_LOCALE(8201, Rtf::eCountry_Jamaica, Rtf::eLanguage_English);
		ADD_LOCALE(17417, Rtf::eCountry_Malaysia, Rtf::eLanguage_English);
		ADD_LOCALE(5129, Rtf::eCountry_NewZealand, Rtf::eLanguage_English);
		ADD_LOCALE(13321, Rtf::eCountry_Philippines, Rtf::eLanguage_English);
		ADD_LOCALE(18441, Rtf::eCountry_Singapore, Rtf::eLanguage_English);
		ADD_LOCALE(7177, Rtf::eCountry_SouthAfrica, Rtf::eLanguage_English);
		ADD_LOCALE(11273, Rtf::eCountry_TrinidadAndTobago, Rtf::eLanguage_English);
		ADD_LOCALE(2057, Rtf::eCountry_UnitedKingdom, Rtf::eLanguage_English);
		ADD_LOCALE(1033, Rtf::eCountry_UnitedStates, Rtf::eLanguage_English);
		ADD_LOCALE(11297, Rtf::eCountry_Zimbabwe, Rtf::eLanguage_English);
		ADD_LOCALE(1061, Rtf::eCountry_Estonia, Rtf::eLanguage_Estonian);
		ADD_LOCALE(1080, Rtf::eCountry_FaroeIslands, Rtf::eLanguage_Faroese);
		ADD_LOCALE(1124, Rtf::eCountry_Philippines, Rtf::eLanguage_Filipino);
		ADD_LOCALE(1035, Rtf::eCountry_Finland, Rtf::eLanguage_Finnish);
		ADD_LOCALE(2060, Rtf::eCountry_Belgium, Rtf::eLanguage_French);
		ADD_LOCALE(11276, Rtf::eCountry_Cameroon, Rtf::eLanguage_French);
		ADD_LOCALE(3084, Rtf::eCountry_Canada, Rtf::eLanguage_French);
		ADD_LOCALE(9228, Rtf::eCountry_CongoDemocraticRepublicOf, Rtf::eLanguage_French);
		ADD_LOCALE(12300, Rtf::eCountry_CotedIvoire, Rtf::eLanguage_French);
		ADD_LOCALE(1036, Rtf::eCountry_France, Rtf::eLanguage_French);
		ADD_LOCALE(15372, Rtf::eCountry_Haiti, Rtf::eLanguage_French);
		ADD_LOCALE(5132, Rtf::eCountry_Luxembourg, Rtf::eLanguage_French);
		ADD_LOCALE(13324, Rtf::eCountry_Mali, Rtf::eLanguage_French);
		ADD_LOCALE(6156, Rtf::eCountry_Monaco, Rtf::eLanguage_French);
		ADD_LOCALE(14348, Rtf::eCountry_Morocco, Rtf::eLanguage_French);
		ADD_LOCALE(8204, Rtf::eCountry_Reunion, Rtf::eLanguage_French);
		ADD_LOCALE(10252, Rtf::eCountry_Senegal, Rtf::eLanguage_French);
		ADD_LOCALE(4108, Rtf::eCountry_Switzerland, Rtf::eLanguage_French);
		ADD_LOCALE(7180, Rtf::eCountry_WestIndies, Rtf::eLanguage_French);
		ADD_LOCALE(1132, Rtf::eCountry_Netherlands, Rtf::eLanguage_Frisian);
		ADD_LOCALE(1127, Rtf::eCountry_None, Rtf::eLanguage_Fulfukle);
		ADD_LOCALE(2108, Rtf::eCountry_Ireland, Rtf::eLanguage_Gaelic);
		ADD_LOCALE(1084, Rtf::eCountry_Scotland, Rtf::eLanguage_Gaelic);
		ADD_LOCALE(1079, Rtf::eCountry_Georgia, Rtf::eLanguage_Georgian);
		ADD_LOCALE(3079, Rtf::eCountry_Austria, Rtf::eLanguage_German);
		ADD_LOCALE(1031, Rtf::eCountry_Germany, Rtf::eLanguage_German);
		ADD_LOCALE(5127, Rtf::eCountry_Liechtenstein, Rtf::eLanguage_German);
		ADD_LOCALE(4103, Rtf::eCountry_Luxembourg, Rtf::eLanguage_German);
		ADD_LOCALE(2055, Rtf::eCountry_Switzerland, Rtf::eLanguage_German);
		ADD_LOCALE(1032, Rtf::eCountry_Greece, Rtf::eLanguage_Greek);
		ADD_LOCALE(1135, Rtf::eCountry_Greenland, Rtf::eLanguage_Greenlandic);
		ADD_LOCALE(1140, Rtf::eCountry_None, Rtf::eLanguage_Guarani);
		ADD_LOCALE(1095, Rtf::eCountry_India, Rtf::eLanguage_Gujarati);
		ADD_LOCALE(1128, Rtf::eCountry_Nigeria, Rtf::eLanguage_HausaLatin);
		ADD_LOCALE(1141, Rtf::eCountry_None, Rtf::eLanguage_Hawaiian);
		ADD_LOCALE(1037, Rtf::eCountry_Israel, Rtf::eLanguage_Hebrew);
		ADD_LOCALE(1081, Rtf::eCountry_India, Rtf::eLanguage_Hindi);
		ADD_LOCALE(1038, Rtf::eCountry_Hungary, Rtf::eLanguage_Hungarian);
		ADD_LOCALE(1129, Rtf::eCountry_Nigeria, Rtf::eLanguage_Ibibio);
		ADD_LOCALE(1039, Rtf::eCountry_Iceland, Rtf::eLanguage_Icelandic);
		ADD_LOCALE(1136, Rtf::eCountry_Nigeria, Rtf::eLanguage_Igbo);
		ADD_LOCALE(2141, Rtf::eCountry_Canada, Rtf::eLanguage_InuktitutLatin);
		ADD_LOCALE(1117, Rtf::eCountry_Canada, Rtf::eLanguage_InuktitutSyllabics);
		ADD_LOCALE(1076, Rtf::eCountry_SouthAfrica, Rtf::eLanguage_IsiXhosa);
		ADD_LOCALE(1077, Rtf::eCountry_SouthAfrica, Rtf::eLanguage_IsiZulu);
		ADD_LOCALE(1040, Rtf::eCountry_Italy, Rtf::eLanguage_Italian);
		ADD_LOCALE(2064, Rtf::eCountry_Switzerland, Rtf::eLanguage_Italian);
		ADD_LOCALE(1041, Rtf::eCountry_Japan, Rtf::eLanguage_Japanese);
		ADD_LOCALE(1099, Rtf::eCountry_India, Rtf::eLanguage_Kannada);
		ADD_LOCALE(1137, Rtf::eCountry_None, Rtf::eLanguage_Kanuri);
		ADD_LOCALE(2144, Rtf::eCountry_None, Rtf::eLanguage_KashmiriLatin);
		ADD_LOCALE(1120, Rtf::eCountry_None, Rtf::eLanguage_KashmiriArabic);
		ADD_LOCALE(1087, Rtf::eCountry_Kazakhstan, Rtf::eLanguage_Kazakh);
		ADD_LOCALE(1107, Rtf::eCountry_Cambodia, Rtf::eLanguage_Khmer);
		ADD_LOCALE(1158, Rtf::eCountry_Guatemala, Rtf::eLanguage_Kiche);
		ADD_LOCALE(1159, Rtf::eCountry_Rwanda, Rtf::eLanguage_Kinyarwanda);
		ADD_LOCALE(1089, Rtf::eCountry_Kenya, Rtf::eLanguage_Kiswahili);
		ADD_LOCALE(1111, Rtf::eCountry_India, Rtf::eLanguage_Konkani);
		ADD_LOCALE(1042, Rtf::eCountry_Korea, Rtf::eLanguage_Korean);
		ADD_LOCALE(1088, Rtf::eCountry_Kyrgyzstan, Rtf::eLanguage_Kyrgyz);
		ADD_LOCALE(1108, Rtf::eCountry_LaoPeoplesDemocraticRepublicOf, Rtf::eLanguage_Lao);
		ADD_LOCALE(1142, Rtf::eCountry_None, Rtf::eLanguage_Latin);
		ADD_LOCALE(1062, Rtf::eCountry_Latvia, Rtf::eLanguage_Latvian);
		ADD_LOCALE(1063, Rtf::eCountry_Lithuania, Rtf::eLanguage_Lithuanian);
		ADD_LOCALE(2087, Rtf::eCountry_None, Rtf::eLanguage_LithuanianTrad);
		ADD_LOCALE(2094, Rtf::eCountry_Germany, Rtf::eLanguage_LowerSorbian);
		ADD_LOCALE(1071, Rtf::eCountry_Macedonia, Rtf::eLanguage_Macedonian);
		ADD_LOCALE(2110, Rtf::eCountry_BruneiDarussalam, Rtf::eLanguage_Malay);
		ADD_LOCALE(1086, Rtf::eCountry_Malaysia, Rtf::eLanguage_Malay);
		ADD_LOCALE(1100, Rtf::eCountry_India, Rtf::eLanguage_Malayalam);
		ADD_LOCALE(1125, Rtf::eCountry_None, Rtf::eLanguage_Maldivian);
		ADD_LOCALE(1082, Rtf::eCountry_Malta, Rtf::eLanguage_Maltese);
		ADD_LOCALE(1112, Rtf::eCountry_None, Rtf::eLanguage_Manipuri);
		ADD_LOCALE(1153, Rtf::eCountry_NewZealand, Rtf::eLanguage_Maori);
		ADD_LOCALE(1146, Rtf::eCountry_Chile, Rtf::eLanguage_Mapudungun);
		ADD_LOCALE(1102, Rtf::eCountry_India, Rtf::eLanguage_Marathi);
		ADD_LOCALE(1148, Rtf::eCountry_Mohawk, Rtf::eLanguage_Mohawk);
		ADD_LOCALE(1104, Rtf::eCountry_Mongolia, Rtf::eLanguage_MonogolianCyrillic);
		ADD_LOCALE(2128, Rtf::eCountry_ChinaPeoplesRepublicOf, Rtf::eLanguage_MonogolianTraditional);
		ADD_LOCALE(2145, Rtf::eCountry_India, Rtf::eLanguage_Nepali);
		ADD_LOCALE(0, Rtf::eCountry_None, Rtf::eLanguage_None);
		ADD_LOCALE(1024, Rtf::eCountry_None, Rtf::eLanguage_None);
		ADD_LOCALE(1044, Rtf::eCountry_Norway, Rtf::eLanguage_NorwegianBokmal);
		ADD_LOCALE(2068, Rtf::eCountry_Norway, Rtf::eLanguage_NorwegianNynorsk);
		ADD_LOCALE(1154, Rtf::eCountry_France, Rtf::eLanguage_Occitan);
		ADD_LOCALE(1096, Rtf::eCountry_India, Rtf::eLanguage_Oriya);
		ADD_LOCALE(1138, Rtf::eCountry_None, Rtf::eLanguage_Oromo);
		ADD_LOCALE(1145, Rtf::eCountry_None, Rtf::eLanguage_Papiamentu);
		ADD_LOCALE(1123, Rtf::eCountry_Afghanistan, Rtf::eLanguage_Pashto);
		ADD_LOCALE(1065, Rtf::eCountry_None, Rtf::eLanguage_Persian);
		ADD_LOCALE(1045, Rtf::eCountry_Poland, Rtf::eLanguage_Polish);
		ADD_LOCALE(1046, Rtf::eCountry_Brazil, Rtf::eLanguage_Portuguese);
		ADD_LOCALE(2070, Rtf::eCountry_Portugal, Rtf::eLanguage_Portuguese);
		ADD_LOCALE(1094, Rtf::eCountry_India, Rtf::eLanguage_Punjabi);
		ADD_LOCALE(2118, Rtf::eCountry_Pakistan, Rtf::eLanguage_Punjabi);
		ADD_LOCALE(1131, Rtf::eCountry_Bolivia, Rtf::eLanguage_Quechua);
		ADD_LOCALE(2155, Rtf::eCountry_Ecuador, Rtf::eLanguage_Quechua);
		ADD_LOCALE(3179, Rtf::eCountry_Peru, Rtf::eLanguage_Quechua);
		ADD_LOCALE(2072, Rtf::eCountry_Moldova, Rtf::eLanguage_Romanian);
		ADD_LOCALE(1048, Rtf::eCountry_Romania, Rtf::eLanguage_Romanian);
		ADD_LOCALE(1047, Rtf::eCountry_Switzerland, Rtf::eLanguage_Romansh);
		ADD_LOCALE(2073, Rtf::eCountry_Moldova, Rtf::eLanguage_Russian);
		ADD_LOCALE(1049, Rtf::eCountry_Russia, Rtf::eLanguage_Russian);
		ADD_LOCALE(9275, Rtf::eCountry_Finland, Rtf::eLanguage_SamiInari);
		ADD_LOCALE(4155, Rtf::eCountry_Norway, Rtf::eLanguage_SamiLule);
		ADD_LOCALE(5179, Rtf::eCountry_Sweden, Rtf::eLanguage_SamiLule);
		ADD_LOCALE(3131, Rtf::eCountry_Finland, Rtf::eLanguage_SamiNorthern);
		ADD_LOCALE(1083, Rtf::eCountry_Norway, Rtf::eLanguage_SamiNorthern);
		ADD_LOCALE(2107, Rtf::eCountry_Sweden, Rtf::eLanguage_SamiNorthern);
		ADD_LOCALE(8251, Rtf::eCountry_Finland, Rtf::eLanguage_SamiSkolt);
		ADD_LOCALE(6203, Rtf::eCountry_Norway, Rtf::eLanguage_SamiSouthern);
		ADD_LOCALE(7227, Rtf::eCountry_Sweden, Rtf::eLanguage_SamiSouthern);
		ADD_LOCALE(1103, Rtf::eCountry_India, Rtf::eLanguage_Sanskrit);
		ADD_LOCALE(7194, Rtf::eCountry_BosniaAndHerzegovina, Rtf::eLanguage_SerbianCyrillic);
		ADD_LOCALE(3098, Rtf::eCountry_Serbia, Rtf::eLanguage_SerbianCyrillic);
		ADD_LOCALE(6170, Rtf::eCountry_BosniaAndHerzegovina, Rtf::eLanguage_SerbianLatin);
		ADD_LOCALE(2074, Rtf::eCountry_Serbia, Rtf::eLanguage_SerbianLatin);
		ADD_LOCALE(1132, Rtf::eCountry_SouthAfrica, Rtf::eLanguage_SesothoSaLeboa);
		ADD_LOCALE(1074, Rtf::eCountry_SouthAfrica, Rtf::eLanguage_Setswana);
		ADD_LOCALE(2137, Rtf::eCountry_None, Rtf::eLanguage_SindhiArabic);
		ADD_LOCALE(1113, Rtf::eCountry_None, Rtf::eLanguage_SindhiDevanagari);
		ADD_LOCALE(1115, Rtf::eCountry_SriLanka, Rtf::eLanguage_Sinhala);
		ADD_LOCALE(1051, Rtf::eCountry_Slovakia, Rtf::eLanguage_Slovak);
		ADD_LOCALE(1060, Rtf::eCountry_Slovenia, Rtf::eLanguage_Slovenian);
		ADD_LOCALE(1143, Rtf::eCountry_None, Rtf::eLanguage_Somali);
		ADD_LOCALE(11274, Rtf::eCountry_Argentina, Rtf::eLanguage_Spanish);
		ADD_LOCALE(16394, Rtf::eCountry_Bolivia, Rtf::eLanguage_Spanish);
		ADD_LOCALE(13322, Rtf::eCountry_Chile, Rtf::eLanguage_Spanish);
		ADD_LOCALE(9226, Rtf::eCountry_Colombia, Rtf::eLanguage_Spanish);
		ADD_LOCALE(5130, Rtf::eCountry_CostaRica, Rtf::eLanguage_Spanish);
		ADD_LOCALE(7178, Rtf::eCountry_DominicanRepublic, Rtf::eLanguage_Spanish);
		ADD_LOCALE(12298, Rtf::eCountry_Ecuador, Rtf::eLanguage_Spanish);
		ADD_LOCALE(17418, Rtf::eCountry_ElSalvador, Rtf::eLanguage_Spanish);
		ADD_LOCALE(4106, Rtf::eCountry_Guatemala, Rtf::eLanguage_Spanish);
		ADD_LOCALE(18442, Rtf::eCountry_Honduras, Rtf::eLanguage_Spanish);
		ADD_LOCALE(2058, Rtf::eCountry_Mexico, Rtf::eLanguage_Spanish);
		ADD_LOCALE(19466, Rtf::eCountry_Nicaragua, Rtf::eLanguage_Spanish);
		ADD_LOCALE(6154, Rtf::eCountry_Panama, Rtf::eLanguage_Spanish);
		ADD_LOCALE(15370, Rtf::eCountry_Paraguay, Rtf::eLanguage_Spanish);
		ADD_LOCALE(10250, Rtf::eCountry_Peru, Rtf::eLanguage_Spanish);
		ADD_LOCALE(20490, Rtf::eCountry_PuertoRico, Rtf::eLanguage_Spanish);
		ADD_LOCALE(3082, Rtf::eCountry_Spain, Rtf::eLanguage_SpanishInternational);
		ADD_LOCALE(1034, Rtf::eCountry_Spain, Rtf::eLanguage_SpanishTraditional);
		ADD_LOCALE(21514, Rtf::eCountry_UnitedStates, Rtf::eLanguage_Spanish);
		ADD_LOCALE(14346, Rtf::eCountry_Uruguay, Rtf::eLanguage_Spanish);
		ADD_LOCALE(8202, Rtf::eCountry_Venezuela, Rtf::eLanguage_Spanish);
		ADD_LOCALE(1072, Rtf::eCountry_SouthAfrica, Rtf::eLanguage_Sutu);
		ADD_LOCALE(2077, Rtf::eCountry_Finland, Rtf::eLanguage_Swedish);
		ADD_LOCALE(1053, Rtf::eCountry_Sweden, Rtf::eLanguage_Swedish);
		ADD_LOCALE(1114, Rtf::eCountry_Syria, Rtf::eLanguage_Syriac);
		ADD_LOCALE(1064, Rtf::eCountry_Tajik, Rtf::eLanguage_TajikCyrillic);
		ADD_LOCALE(1119, Rtf::eCountry_Morocco, Rtf::eLanguage_TamazightArabic);
		ADD_LOCALE(2143, Rtf::eCountry_Algeria, Rtf::eLanguage_TamazightLatin);
		ADD_LOCALE(1097, Rtf::eCountry_India, Rtf::eLanguage_Tamil);
		ADD_LOCALE(1092, Rtf::eCountry_Russia, Rtf::eLanguage_Tatar);
		ADD_LOCALE(1098, Rtf::eCountry_India, Rtf::eLanguage_Telugu);
		ADD_LOCALE(1054, Rtf::eCountry_Thailand, Rtf::eLanguage_Thai);
		ADD_LOCALE(1105, Rtf::eCountry_ChinaPeoplesRepublicOf, Rtf::eLanguage_Tibetan);
		ADD_LOCALE(2163, Rtf::eCountry_Eritrea, Rtf::eLanguage_Tigrigna);
		ADD_LOCALE(1139, Rtf::eCountry_Ethiopia, Rtf::eLanguage_Tigrigna);
		ADD_LOCALE(1073, Rtf::eCountry_None, Rtf::eLanguage_Tsonga);
		ADD_LOCALE(1055, Rtf::eCountry_Turkey, Rtf::eLanguage_Turkish);
		ADD_LOCALE(1090, Rtf::eCountry_Turkmenistan, Rtf::eLanguage_Turkmen);
		ADD_LOCALE(1152, Rtf::eCountry_ChinaPeoplesRepublicOf, Rtf::eLanguage_Uighur);
		ADD_LOCALE(1058, Rtf::eCountry_Ukraine, Rtf::eLanguage_Ukrainian);
		ADD_LOCALE(1070, Rtf::eCountry_Germany, Rtf::eLanguage_UpperSorbian);
		ADD_LOCALE(1056, Rtf::eCountry_Pakistan, Rtf::eLanguage_Urdu);
		ADD_LOCALE(2080, Rtf::eCountry_India, Rtf::eLanguage_Urdu);
		ADD_LOCALE(2115, Rtf::eCountry_Uzbekistan, Rtf::eLanguage_UzbekCyrillic);
		ADD_LOCALE(1091, Rtf::eCountry_Uzbekistan, Rtf::eLanguage_UzbekLatin);
		ADD_LOCALE(1075, Rtf::eCountry_None, Rtf::eLanguage_Venda);
		ADD_LOCALE(1066, Rtf::eCountry_Vietnam, Rtf::eLanguage_Vietnamese);
		ADD_LOCALE(1106, Rtf::eCountry_UnitedKingdom, Rtf::eLanguage_Welsh);
		ADD_LOCALE(1160, Rtf::eCountry_Senegal, Rtf::eLanguage_Wolof);
		ADD_LOCALE(1076, Rtf::eCountry_None, Rtf::eLanguage_Xhosa);
		ADD_LOCALE(1157, Rtf::eCountry_Russia, Rtf::eLanguage_Yakut);
		ADD_LOCALE(1144, Rtf::eCountry_ChinaPeoplesRepublicOf, Rtf::eLanguage_Yi);
		ADD_LOCALE(1085, Rtf::eCountry_None, Rtf::eLanguage_Yiddish);
		ADD_LOCALE(1130, Rtf::eCountry_Nigeria, Rtf::eLanguage_Yoruba);

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

	unsigned int RtfWorld::GetLocaleIdentifierByLanguageAndCountry(Rtf::Language a_Language, Rtf::Country a_Country) const
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

	std::set<Rtf::Language> RtfWorld::GetLanguagesForCountry(Rtf::Country a_Country) const
	{
		std::set<Rtf::Language> results;

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

	std::set<Rtf::Country> RtfWorld::GetCountriesForLanguage(Rtf::Language a_Language) const
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