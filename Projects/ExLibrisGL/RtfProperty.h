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

	template <typename Type, unsigned int Bit>
	class RtfProperty
	{

	public:

		RtfProperty(const Type& a_Value, unsigned int& a_Specified)
			: m_Data(a_Value)
			, m_Specified(a_Specified)
		{
		}

		RtfProperty(const RtfProperty<Type, Bit>& a_Other)
			: m_Data(a_Other.m_Data)
			, m_Specified(a_Other.m_Specified)
		{
		}

		inline bool IsSet() const
		{
			return ((m_Specified & (1 << Bit)) != 0);
		}

		inline const Type& Get() const
		{
			return m_Data;
		}

		inline void operator = (const Type& a_Value)
		{
			m_Data = a_Value;
			m_Specified |= (1 << Bit);
		}

		inline void operator = (const RtfProperty<Type, Bit>& a_Other)
		{
			if (a_Other.IsSet())
			{
				m_Data = a_Other.m_Data;
				m_Specified |= (1 << Bit);
			}
		}

	private:

		Type m_Data;
		unsigned int& m_Specified;

	}; // class RtfProperty

}; // namespace ExLibris