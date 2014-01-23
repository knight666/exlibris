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

namespace ExLibris {
namespace Rtf {

	enum CharacterEncoding
	{
		eCharacterEncoding_SingleByteLowAnsi,
		eCharacterEncoding_SingleByteHighAnsi,
		eCharacterEncoding_DoubleByte
	};

	enum CharacterSet
	{
		eCharacterSet_Invalid,
		eCharacterSet_Ansi,
		eCharacterSet_Default,
		eCharacterSet_Symbol,
		eCharacterSet_AppleMacintoshRoman,
		eCharacterSet_AppleMacintoshShiftJis,
		eCharacterSet_AppleMacintoshHangul,
		eCharacterSet_AppleMacintoshGb2332,
		eCharacterSet_AppleMacintoshBig5,
		eCharacterSet_AppleMacintoshJohabOld,
		eCharacterSet_AppleMacintoshHebrew,
		eCharacterSet_AppleMacintoshArabic,
		eCharacterSet_AppleMacintoshGreek,
		eCharacterSet_AppleMacintoshTurkish,
		eCharacterSet_AppleMacintoshThai,
		eCharacterSet_AppleMacintoshEasternEuropean,
		eCharacterSet_AppleMacintoshRussian,
		eCharacterSet_ShiftJis,
		eCharacterSet_Hangul,
		eCharacterSet_Johab,
		eCharacterSet_Gb2332,
		eCharacterSet_Big5,
		eCharacterSet_Greek,
		eCharacterSet_Turkish,
		eCharacterSet_Vietnamese,
		eCharacterSet_Hebrew,
		eCharacterSet_Arabic,
		eCharacterSet_ArabicTraditionalOld,
		eCharacterSet_ArabicUserOld,
		eCharacterSet_HebrewUserOld,
		eCharacterSet_Baltic,
		eCharacterSet_Russian,
		eCharacterSet_Thai,
		eCharacterSet_EasternEuropean,
		eCharacterSet_IbmPcCodePage437,
		eCharacterSet_Oem,
	};

	enum Country
	{
		eCountry_None,
		eCountry_Current,
		eCountry_Preferred,
		eCountry_Afghanistan,
		eCountry_Albania,
		eCountry_Algeria,
		eCountry_Argentina,
		eCountry_Armenia,
		eCountry_Australia,
		eCountry_Austria,
		eCountry_Azerbaijan,
		eCountry_Bahrain,
		eCountry_Bangladesh,
		eCountry_Basque,
		eCountry_Belarus,
		eCountry_Belgium,
		eCountry_Belize,
		eCountry_Bolivia,
		eCountry_BosniaAndHerzegovina,
		eCountry_Brazil,
		eCountry_BruneiDarussalam,
		eCountry_Bulgaria,
		eCountry_Cambodia,
		eCountry_Cameroon,
		eCountry_Canada,
		eCountry_Caribbean,
		eCountry_Catalan,
		eCountry_Chile,
		eCountry_ChinaPeoplesRepublicOf,
		eCountry_Colombia,
		eCountry_CongoDemocraticRepublicOf,
		eCountry_CostaRica,
		eCountry_CotedIvoire,
		eCountry_Croatia,
		eCountry_CzechRepublic,
		eCountry_Denmark,
		eCountry_DominicanRepublic,
		eCountry_Ecuador,
		eCountry_Egypt,
		eCountry_ElSalvador,
		eCountry_Estonia,
		eCountry_Ethiopia,
		eCountry_Eritrea,
		eCountry_FaroeIslands,
		eCountry_Finland,
		eCountry_France,
		eCountry_Georgia,
		eCountry_Germany,
		eCountry_Greece,
		eCountry_Greenland,
		eCountry_Guatemala,
		eCountry_Haiti,
		eCountry_Honduras,
		eCountry_Hungary,
		eCountry_Iceland,
		eCountry_India,
		eCountry_Indonesia,
		eCountry_Iraq,
		eCountry_Ireland,
		eCountry_Israel,
		eCountry_Italy,
		eCountry_Jamaica,
		eCountry_Japan,
		eCountry_Jordan,
		eCountry_Kenya,
		eCountry_Korea,
		eCountry_Kuwait,
		eCountry_Kyrgyzstan,
		eCountry_LaoPeoplesDemocraticRepublicOf,
		eCountry_Latvia,
		eCountry_Lebanon,
		eCountry_Libya,
		eCountry_Liechtenstein,
		eCountry_Lithuania,
		eCountry_Luxembourg,
		eCountry_Kazakhstan,
		eCountry_Macedonia,
		eCountry_Malaysia,
		eCountry_Maldives,
		eCountry_Mali,
		eCountry_Malta,
		eCountry_Mexico,
		eCountry_Mohawk,
		eCountry_Moldova,
		eCountry_Monaco,
		eCountry_Mongolia,
		eCountry_Morocco,
		eCountry_Nepal,
		eCountry_Netherlands,
		eCountry_NewZealand,
		eCountry_Nicaragua,
		eCountry_Nigeria,
		eCountry_Norway,
		eCountry_Oman,
		eCountry_Pakistan,
		eCountry_Panama,
		eCountry_Paraguay,
		eCountry_Peru,
		eCountry_Philippines,
		eCountry_Poland,
		eCountry_Portugal,
		eCountry_PuertoRico,
		eCountry_Qatar,
		eCountry_Reunion,
		eCountry_Romania,
		eCountry_Rwanda,
		eCountry_Russia,
		eCountry_SaudiArabia,
		eCountry_Scotland,
		eCountry_Senegal,
		eCountry_Serbia,
		eCountry_Singapore,
		eCountry_Slovakia,
		eCountry_Slovenia,
		eCountry_Spain,
		eCountry_SpecialAdministrativeRegionHongKong,
		eCountry_SpecialAdministrativeRegionMacao,
		eCountry_SriLanka,
		eCountry_SouthAfrica,
		eCountry_Sweden,
		eCountry_Switzerland,
		eCountry_Syria,
		eCountry_Taiwan,
		eCountry_Tajik,
		eCountry_Thailand,
		eCountry_TrinidadAndTobago,
		eCountry_Tunisia,
		eCountry_Turkey,
		eCountry_Turkmenistan,
		eCountry_Ukraine,
		eCountry_UnitedArabicEmirates,
		eCountry_UnitedKingdom,
		eCountry_UnitedStates,
		eCountry_Uruguay,
		eCountry_Uzbekistan,
		eCountry_Venezuela,
		eCountry_Vietnam,
		eCountry_WestIndies,
		eCountry_Yemen,
		eCountry_Zimbabwe,
	};

	enum FamilyType
	{
		eFamilyType_Nil,
		eFamilyType_Roman,
		eFamilyType_Swiss,
		eFamilyType_Modern,
		eFamilyType_Script,
		eFamilyType_Decor,
		eFamilyType_Tech,
		eFamilyType_Bidi,
	};

	enum Language
	{
		eLanguage_None,
		eLanguage_Custom,
		eLanguage_Afrikaans,
		eLanguage_Albanian,
		eLanguage_Alsatian,
		eLanguage_Amharic,
		eLanguage_Arabic,
		eLanguage_Armenian,
		eLanguage_Assamese,
		eLanguage_AzeriCyrillic,
		eLanguage_AzeriLatin,
		eLanguage_Bashkir,
		eLanguage_Basque,
		eLanguage_Belarusian,
		eLanguage_Bengali,
		eLanguage_BosnianCyrillic,
		eLanguage_BosnianLatin,
		eLanguage_Breton,
		eLanguage_Bulgarian,
		eLanguage_Burmese,
		eLanguage_Catalan,
		eLanguage_Cherokee,
		eLanguage_Chinese,
		eLanguage_Corsican,
		eLanguage_Croatian,
		eLanguage_CroatianLatin,
		eLanguage_Czech,
		eLanguage_Danish,
		eLanguage_Dari,
		eLanguage_Divehi,
		eLanguage_Dutch,
		eLanguage_Dzongkha,
		eLanguage_Edo,
		eLanguage_English,
		eLanguage_Estonian,
		eLanguage_Faroese,
		eLanguage_Filipino,
		eLanguage_Finnish,
		eLanguage_French,
		eLanguage_Frisian,
		eLanguage_Fulfukle,
		eLanguage_Gaelic,
		eLanguage_Georgian,
		eLanguage_German,
		eLanguage_Greek,
		eLanguage_Greenlandic,
		eLanguage_Guarani,
		eLanguage_Gujarati,
		eLanguage_HausaLatin,
		eLanguage_Hawaiian,
		eLanguage_Hebrew,
		eLanguage_Hindi,
		eLanguage_Hungarian,
		eLanguage_Ibibio,
		eLanguage_Icelandic,
		eLanguage_Igbo,
		eLanguage_Indonesian,
		eLanguage_InuktitutLatin,
		eLanguage_InuktitutSyllabics,
		eLanguage_IsiXhosa,
		eLanguage_IsiZulu,
		eLanguage_Italian,
		eLanguage_Japanese,
		eLanguage_Kannada,
		eLanguage_Kanuri,
		eLanguage_KashmiriArabic,
		eLanguage_KashmiriLatin,
		eLanguage_Kazakh,
		eLanguage_Khmer,
		eLanguage_Kiche,
		eLanguage_Kinyarwanda,
		eLanguage_Kiswahili,
		eLanguage_Konkani,
		eLanguage_Korean,
		eLanguage_Kyrgyz,
		eLanguage_Lao,
		eLanguage_Latin,
		eLanguage_Latvian,
		eLanguage_Lithuanian,
		eLanguage_LithuanianTrad,
		eLanguage_LowerSorbian,
		eLanguage_Luxembourgish,
		eLanguage_Macedonian,
		eLanguage_Malay,
		eLanguage_Malayalam,
		eLanguage_Maldivian,
		eLanguage_Maltese,
		eLanguage_Manipuri,
		eLanguage_Maori,
		eLanguage_Mapudungun,
		eLanguage_Marathi,
		eLanguage_Mohawk,
		eLanguage_MonogolianCyrillic,
		eLanguage_MonogolianTraditional,
		eLanguage_Nepali,
		eLanguage_NorwegianBokmal,
		eLanguage_NorwegianNynorsk,
		eLanguage_Occitan,
		eLanguage_Oriya,
		eLanguage_Oromo,
		eLanguage_Papiamentu,
		eLanguage_Pashto,
		eLanguage_Persian,
		eLanguage_Polish,
		eLanguage_Portuguese,
		eLanguage_Punjabi,
		eLanguage_Quechua,
		eLanguage_Romanian,
		eLanguage_Romansh,
		eLanguage_Russian,
		eLanguage_SamiInari,
		eLanguage_SamiLule,
		eLanguage_SamiNorthern,
		eLanguage_SamiSkolt,
		eLanguage_SamiSouthern,
		eLanguage_Sanskrit,
		eLanguage_SerbianCyrillic,
		eLanguage_SerbianLatin,
		eLanguage_SesothoSaLeboa,
		eLanguage_Setswana,
		eLanguage_SindhiArabic,
		eLanguage_SindhiDevanagari,
		eLanguage_Sinhala,
		eLanguage_Slovak,
		eLanguage_Slovenian,
		eLanguage_Somali,
		eLanguage_Spanish,
		eLanguage_SpanishInternational,
		eLanguage_SpanishTraditional,
		eLanguage_Sutu,
		eLanguage_Swedish,
		eLanguage_Syriac,
		eLanguage_TajikCyrillic,
		eLanguage_TamazightArabic,
		eLanguage_TamazightLatin,
		eLanguage_Tamil,
		eLanguage_Tatar,
		eLanguage_Telugu,
		eLanguage_Thai,
		eLanguage_Tibetan,
		eLanguage_Tigrigna,
		eLanguage_Tsonga,
		eLanguage_Turkish,
		eLanguage_Turkmen,
		eLanguage_Uighur,
		eLanguage_Ukrainian,
		eLanguage_UpperSorbian,
		eLanguage_Urdu,
		eLanguage_UzbekCyrillic,
		eLanguage_UzbekLatin,
		eLanguage_Venda,
		eLanguage_Vietnamese,
		eLanguage_Welsh,
		eLanguage_Wolof,
		eLanguage_Xhosa,
		eLanguage_Yakut,
		eLanguage_Yi,
		eLanguage_Yiddish,
		eLanguage_Yoruba,
	};

	enum Pitch
	{
		ePitch_Default  = 0,
		ePitch_Fixed    = 1,
		ePitch_Variable = 2
	};

}; // namespace Rtf
}; // namespace ExLibris