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

	enum Pitch
	{
		ePitch_Default  = 0,
		ePitch_Fixed    = 1,
		ePitch_Variable = 2
	};

}; // namespace Rtf
}; // namespace ExLibris