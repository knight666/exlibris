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

#include "Token.h"

namespace ExLibris
{

	class Tokenizer
	{

	public:

		enum Option
		{
			eOption_Identifiers = (1 << 0)
		};
	
	public:
	
		Tokenizer(std::basic_istream<char>* a_Stream);
		~Tokenizer();

		bool IsOptionEnabled(Option a_Option) const;
		void EnableOptions(unsigned int a_Options);

		bool IsOptionDisabled(Option a_Option) const;
		void DisableOptions(unsigned int a_Options);

		void SetTabWidth(size_t a_Width);

		void SetInput(std::basic_istream<char>* a_Stream);

		const Token& GetCurrentToken() const;

		bool IsNextTokenAvailable() const;
		bool ReadToken();

	private:

		bool _RecursiveReadToken();

		bool _IsNextCharacterAvailable() const;
		bool _NextCharacter();
		void _Revert(int a_Count);

		inline void _AddToToken(int a_Character)
		{
			m_TokenCurrent.text.push_back((char)a_Character);
			m_CharactersConsumedCount++;
		}
		inline void _AddCurrentToToken()
		{
			_AddToToken(m_CharacterCurrent);
		}

		inline bool _Match(int a_A) const
		{
			return (m_CharacterCurrent == a_A);
		}

		inline bool _MatchEither(int a_A, int a_B) const
		{
			return (m_CharacterCurrent == a_A || m_CharacterCurrent == a_B);
		}

		template<typename CharacterType>
		inline bool _MatchType() const
		{
			return (CharacterType::IsKnown(m_CharacterCurrent));
		}

		inline bool _TryConsume(int a_Character)
		{
			if (m_CharacterCurrent == a_Character)
			{
				_AddCurrentToToken();

				return true;
			}
			else
			{
				return false;
			}
		}

		inline bool _TryConsumeEither(int a_A, int a_B)
		{
			if (m_CharacterCurrent == a_A || m_CharacterCurrent == a_B)
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
		inline bool _TryConsumeType()
		{
			if (CharacterType::IsKnown(m_CharacterCurrent))
			{
				_AddCurrentToToken();

				return true;
			}
			else
			{
				return false;
			}
		}

	private:

		unsigned int m_Options;

		std::basic_istream<char>* m_Stream;
		bool m_StreamEnd;

		size_t m_TabWidth;

		Token m_TokenCurrent;
		std::deque<int> m_CharactersRead;
		int m_CharactersConsumedCount;

		bool m_NumberFoundDot;
		Token::Type m_ScientificTypeRestore;
		int m_StringDelimiter;

		int m_CharacterCurrent;
		std::deque<int> m_CharacterQueue;

		int m_CharacterRestore;
		std::deque<int> m_CharactersUndoConsumed;

		int m_Column;
		int m_Line;
	
	}; // class Tokenizer

}; // namespace ExLibris