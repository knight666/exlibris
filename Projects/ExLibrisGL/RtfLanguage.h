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

#pragma once

namespace ExLibris
{

	enum RtfLanguage
	{
		eRtfLanguage_None,
		eRtfLanguage_Afrikaans,
		eRtfLanguage_Albanian,
		eRtfLanguage_Alsatian,
		eRtfLanguage_Amharic,
		eRtfLanguage_Arabic,
		eRtfLanguage_Armenian,
		eRtfLanguage_Assamese,
		eRtfLanguage_AzeriCyrillic,
		eRtfLanguage_AzeriLatin,
		eRtfLanguage_Bashkir,
		eRtfLanguage_Basque,
		eRtfLanguage_Belarusian,
		eRtfLanguage_Bengali,
		eRtfLanguage_BosnianCyrillic,
		eRtfLanguage_BosnianLatin,
		eRtfLanguage_Breton,
		eRtfLanguage_Bulgarian,
		eRtfLanguage_Burmese,
		eRtfLanguage_Catalan,
		eRtfLanguage_Chinese,
		eRtfLanguage_Corsican,
		eRtfLanguage_Croatian,
		eRtfLanguage_CroatianLatin,
		eRtfLanguage_Czech,
		eRtfLanguage_Danish,
		eRtfLanguage_Dari,
		eRtfLanguage_Divehi,
		eRtfLanguage_Dutch,
		eRtfLanguage_Dzongkha,
		eRtfLanguage_Edo,
		eRtfLanguage_English,
		eRtfLanguage_Estonian,
		eRtfLanguage_Faroese,
		eRtfLanguage_Filipino,
		eRtfLanguage_Finnish,
		eRtfLanguage_French,
		eRtfLanguage_Frisian,
		eRtfLanguage_Fulfukle,
	};

	/*static const RtfLanguagePair g_LanguageAfrikaansSouthAfrica = 1078;
	static const RtfLanguagePair g_LanguageAlbanianAlbania = 1052;
	static const RtfLanguagePair g_LanguageAlsatianFrance = 1156;
	static const RtfLanguagePair g_LanguageAmharicEthiopa = 1118;
	static const RtfLanguagePair g_LanguageArabicAlgeria = 5121;
	static const RtfLanguagePair g_LanguageArabicBahrain = 15361;
	static const RtfLanguagePair g_LanguageArabicEgypt = 3073;
	static const RtfLanguagePair g_LanguageArabicIraq = 2049;
	static const RtfLanguagePair g_LanguageArabicJordan = 11265;
	static const RtfLanguagePair g_LanguageArabicKuwait = 13313;
	static const RtfLanguagePair g_LanguageArabicLebanon = 12289;
	static const RtfLanguagePair g_LanguageArabicLibya = 4097;
	static const RtfLanguagePair g_LanguageArabicMorocco = 6145;
	static const RtfLanguagePair g_LanguageArabicOman = 8193;
	static const RtfLanguagePair g_LanguageArabicQatar = 16385;
	static const RtfLanguagePair g_LanguageArabicSaudiArabia = 1025;
	static const RtfLanguagePair g_LanguageArabicSyria = 10241;
	static const RtfLanguagePair g_LanguageArabicTunisia = 7169;
	static const RtfLanguagePair g_LanguageArabicUnitedArabicEmirates = 14337;
	static const RtfLanguagePair g_LanguageArabicYemen = 9217;
	static const RtfLanguagePair g_LanguageArmenianArmenia = 1067;
	static const RtfLanguagePair g_LanguageAssameseIndia = 1101;
	static const RtfLanguagePair g_LanguageAzeriCyrillicAzerbaijan = 2092;
	static const RtfLanguagePair g_LanguageAzeriLatinAzerbaijan = 1068;
	static const RtfLanguagePair g_LanguageBashkirRussia = 1133;
	static const RtfLanguagePair g_LanguageBasqueBasque = 1069;
	static const RtfLanguagePair g_LanguageBelarusianBelarus = 1059;
	static const RtfLanguagePair g_LanguageBengaliBangladesh = 2117;
	static const RtfLanguagePair g_LanguageBengaliIndia = 1093;
	static const RtfLanguagePair g_LanguageBosnian = 4122;
	static const RtfLanguagePair g_LanguageBosnianCyrillicBosniaHerzegovina = 8218;
	static const RtfLanguagePair g_LanguageBosnianLatinBosniaHerzegovina = 5146;
	static const RtfLanguagePair g_LanguageBretonFrance = 1150;
	static const RtfLanguagePair g_LanguageBulgarianBulgaria = 1026;
	static const RtfLanguagePair g_LanguageBurmese = 1109;
	static const RtfLanguagePair g_LanguageCatalanCatalan = 1027;
	static const RtfLanguagePair g_LanguageCherokee = 1116;
	static const RtfLanguagePair g_LanguageChineseHongKongSAR = 3076;
	static const RtfLanguagePair g_LanguageChineseMacaoSAR = 5124;
	static const RtfLanguagePair g_LanguageChineseChina = 2052;
	static const RtfLanguagePair g_LanguageChineseSingapore = 4100;
	static const RtfLanguagePair g_LanguageChineseTaiwan = 1028;
	static const RtfLanguagePair g_LanguageCorsicanFrance = 1155;
	static const RtfLanguagePair g_LanguageCroatianCroatia = 1050;
	static const RtfLanguagePair g_LanguageCroatianLatinBosniaHerzegovina = 4122;
	static const RtfLanguagePair g_LanguageCustomCurrent = 3072;
	static const RtfLanguagePair g_LanguageCzechCzechRepublic = 1029;
	static const RtfLanguagePair g_LanguageDanishDenmark = 1030;
	static const RtfLanguagePair g_LanguageDariAfghanistan = 1164;
	static const RtfLanguagePair g_LanguageDivehiMaldives = 1125;
	static const RtfLanguagePair g_LanguageDutchBelgium = 2067;
	static const RtfLanguagePair g_LanguageDutchNetherlands = 1043;
	static const RtfLanguagePair g_LanguageDutchPreferred = 19;
	static const RtfLanguagePair g_LanguageDzongkha = 1126;
	static const RtfLanguagePair g_LanguageEnglishAustralia = 3081;
	static const RtfLanguagePair g_LanguageEnglishBelize = 10249;
	static const RtfLanguagePair g_LanguageEnglishCanada = 4105;
	static const RtfLanguagePair g_LanguageEnglishCaribbean = 9225;
	static const RtfLanguagePair g_LanguageEnglishHongKongSar = 15369;
	static const RtfLanguagePair g_LanguageEnglishIndia = 16393;
	static const RtfLanguagePair g_LanguageEnglishIndonesia = 14345;
	static const RtfLanguagePair g_LanguageEnglishIreland = 6153;
	static const RtfLanguagePair g_LanguageEnglishJamaica = 8201;
	static const RtfLanguagePair g_LanguageEnglishMalaysia = 17417;
	static const RtfLanguagePair g_LanguageEnglishNewZealand = 5129;
	static const RtfLanguagePair g_LanguageEnglishPhilippines = 13321;
	static const RtfLanguagePair g_LanguageEnglishSingapore = 18441;
	static const RtfLanguagePair g_LanguageEnglishSouthAfrica = 7177;
	static const RtfLanguagePair g_LanguageEnglishTrinidadTobago = 11273;
	static const RtfLanguagePair g_LanguageEnglishUnitedKingdom = 2057;
	static const RtfLanguagePair g_LanguageEnglishUnitedStates = 1033;
	static const RtfLanguagePair g_LanguageEnglishZimbabwe = 11297;
	static const RtfLanguagePair g_LanguageEstonianEstonia = 1061;
	static const RtfLanguagePair g_LanguageFaroeseFaroeIslands = 1080;
	static const RtfLanguagePair g_LanguageFilipinoPhilippines = 1124;
	static const RtfLanguagePair g_LanguageFinnishFinland = 1035;
	static const RtfLanguagePair g_LanguageFrenchBelgium = 2060;
	static const RtfLanguagePair g_LanguageFrenchCameroon = 11276;
	static const RtfLanguagePair g_LanguageFrenchCanada = 3084;
	static const RtfLanguagePair g_LanguageFrenchCongo = 9228;
	static const RtfLanguagePair g_LanguageFrenchCotedIvoire = 12300;
	static const RtfLanguagePair g_LanguageFrenchFrance = 1036;
	static const RtfLanguagePair g_LanguageFrenchHaiti = 15372;
	static const RtfLanguagePair g_LanguageFrenchLuxembourg = 5132;
	static const RtfLanguagePair g_LanguageFrenchMali = 13324;
	static const RtfLanguagePair g_LanguageFrenchMonaco = 6156;
	static const RtfLanguagePair g_LanguageFrenchMorocco = 14348;
	static const RtfLanguagePair g_LanguageFrenchReunion = 8204;
	static const RtfLanguagePair g_LanguageFrenchSenegal = 10252;
	static const RtfLanguagePair g_LanguageFrenchSwitzerland = 4108;
	static const RtfLanguagePair g_LanguageFrenchWestIndies = 7180;
	static const RtfLanguagePair g_LanguageFrisianNetherlands = 1132;
	static const RtfLanguagePair g_LanguageFulfukle = 1127;
	static const RtfLanguagePair g_LanguageGaelicIreland = 2108;

	static RtfLanguagePair LanguageIdentifierToPair(unsigned int a_Identifier);*/

}; // namespace ExLibris