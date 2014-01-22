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

#include "RtfUnit.h"

namespace ExLibris
{

	const static float s_FromPoints = 72.0f / 1440.0f;
	const static float s_FromDeviceIndependentUnits = 294912.0f / 1440.0f;
	const static float s_FromEnglishMetricUnits = 914400.0f / 1440.0f;
	const static float s_FromPixels = 96.0f / 1440.0f;

	const static float s_ToPoints = 1440.0f / 72.0f;
	const static float s_ToDeviceIndependentUnits = 1440.0f / 294912.0f;
	const static float s_ToEnglishMetricUnits = 1440.0f / 914400.0f;
	const static float s_ToPixels = 1440.0f / 96.0f;

	RtfUnit::RtfUnit()
		: m_TwipsValue(0.0f)
	{
	}

	RtfUnit::RtfUnit(float a_Twips)
		: m_TwipsValue(a_Twips)
	{
	}

	RtfUnit::RtfUnit(Type a_Type, float a_Value)
	{
		SetValue(a_Type, a_Value);
	}

	float RtfUnit::GetValue(Type a_Type) const
	{
		switch (a_Type)
		{

		case eType_Twips:
			{
				return m_TwipsValue;

			} break;

		case eType_Points:
			{
				return m_TwipsValue * s_ToPoints;

			} break;

		case eType_DeviceIndependentUnits:
			{
				return m_TwipsValue * s_ToDeviceIndependentUnits;

			} break;

		case eType_EnglishMetricUnits:
			{
				return m_TwipsValue * s_ToEnglishMetricUnits;

			} break;

		case eType_Pixels:
			{
				return m_TwipsValue * s_ToPixels;

			} break;

		default:
			{
				return 0.0f;

			} break;

		}
	}

	void RtfUnit::SetValue(Type a_Type, float a_Value)
	{
		switch (a_Type)
		{

		case eType_Twips:
			{
				m_TwipsValue = a_Value;

			} break;

		case eType_Points:
			{
				m_TwipsValue = a_Value * s_FromPoints;

			} break;

		case eType_DeviceIndependentUnits:
			{
				m_TwipsValue = a_Value * s_FromDeviceIndependentUnits;

			} break;

		case eType_EnglishMetricUnits:
			{
				m_TwipsValue = a_Value * s_FromEnglishMetricUnits;

			} break;

		case eType_Pixels:
			{
				m_TwipsValue = a_Value * s_FromPixels;

			} break;

		}
	}

}; // namespace ExLibris"