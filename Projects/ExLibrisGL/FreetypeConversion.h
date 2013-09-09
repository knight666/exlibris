#ifndef _FREETYPECONVERSION_H_
#define _FREETYPECONVERSION_H_

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

#endif