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

	namespace FixedInteger
	{

		inline float ToFloat(FT_Pos a_Value)
		{
			return (float)a_Value;
		}

		inline glm::vec2 ToFloatVec2(const FT_Vector* a_Position)
		{
			return glm::vec2(
				ToFloat(a_Position->x),
				ToFloat(a_Position->y)
			);
		}

	}; // namespace FixedInteger

	namespace Fixed16Dot16
	{

		inline FT_Fixed ToFixed(float a_Value)
		{
			return (FT_Fixed)(a_Value * 65536.0f);
		}
	
		inline float ToFloat(FT_Pos a_Value)
		{
			return (float)a_Value / 65536.0f;
		}

		inline glm::vec2 ToFloatVec2(const FT_Vector* a_Position)
		{
			return glm::vec2(
				ToFloat(a_Position->x),
				ToFloat(a_Position->y)
			);
		}
	
	}; // namespace Fixed16Dot16

	namespace Fixed26Dot6
	{
	
		inline float ToFloat(FT_Pos a_Value)
		{
			return (float)a_Value / 64.0f;
		}

		inline FT_F26Dot6 ToFixed(float a_Value)
		{
			return (FT_F26Dot6)(a_Value * 64.0f);
		}

		inline glm::vec2 ToFloatVec2(const FT_Vector* a_Position)
		{
			return glm::vec2(
				ToFloat(a_Position->x),
				ToFloat(a_Position->y)
			);
		}
	
	}; // namespace Fixed26Dot6

}; // namespace ExLibris