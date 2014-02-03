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

#include "RtfToken.h"
#include <istream>

namespace ExLibris {
namespace Rtf {

	class Tokenizer
	{

	public:

		Tokenizer();
		~Tokenizer();

		void SetInput(std::basic_istream<char>* a_Stream);

		bool IsNextAvailable() const;

		const RtfToken& GetCurrent() const;

		bool Read();

	private:

		bool _NextCharacter();

		inline void _AddCurrentToToken()
		{
			m_Current.value.push_back(m_Character);
			m_Consumed++;
		}

		inline bool _Match(char a_Character) const
		{
			return (m_Character == a_Character);
		}
		template<typename CharacterType>
		inline bool _MatchType() const
		{
			return (CharacterType::Match(m_Character));
		}

		inline bool _Consume(char a_Character)
		{
			if (m_Character == a_Character)
			{
				_AddCurrentToToken();

				return true;
			}
			else
			{
				return false;
			}
		}
		template<typename CharacterType>
		inline bool _ConsumeType()
		{
			if (CharacterType::Match(m_Character))
			{
				_AddCurrentToToken();

				return true;
			}
			else
			{
				return false;
			}
		}

		template<typename CharacterType>
		inline bool _ReadTyped()
		{
			if (_NextCharacter())
			{
				if (_MatchType<CharacterType>())
				{
					return true;
				}
				else
				{
					_RevertCurrent();

					return false;
				}
			}
			else
			{
				return false;
			}
		}

		bool _RecursiveRead();

		inline void _RevertCurrent()
		{
			m_CharacterQueued = m_Character;
			m_Column--;
		}

	private:

		std::basic_istream<char>* m_Input;
		RtfToken m_Current;
		int m_Group;
		int m_Column;
		int m_Line;
		char m_Character;
		char m_CharacterQueued;
		int m_Consumed;

	}; // class Tokenizer

}; // namespace Rtf
}; // namespace ExLibris