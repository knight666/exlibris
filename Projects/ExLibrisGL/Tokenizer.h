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
	
		Tokenizer(std::basic_istream<char>* a_Stream);
		~Tokenizer();

		void SetInput(std::basic_istream<char>* a_Stream);

		const Token& GetCurrentToken() const;

		bool IsNextTokenAvailable() const;
		bool ReadToken();

	private:

		bool _TryReadCharacter();
		void _QueueCurrentCharacter();

		bool _ReadOne();
		bool _ReadOneOrMore();

		template<typename CharacterType>
		bool _IsCharacterOfType(int a_Character);

		template<typename CharacterType>
		bool _TryReadOneOrMore();

		Token::Type _GetTypeForCharacter(int a_Character);

	private:

		std::basic_istream<char>* m_Stream;

		Token m_TokenCurrent;

		int m_CharacterCurrent;
		std::queue<int> m_CharacterQueue;

		int m_Column;
		int m_Line;
	
	}; // class Tokenizer

	template<typename CharacterType>
	inline bool Tokenizer::_IsCharacterOfType(int a_Character)
	{
		return (CharacterType::IsKnown(a_Character));
	}

	template<typename CharacterType>
	inline bool Tokenizer::_TryReadOneOrMore()
	{
		int previous = m_CharacterCurrent;
		int found = 0;

		while (_TryReadCharacter() && _IsCharacterOfType<CharacterType>(m_CharacterCurrent))
		{
			m_TokenCurrent.text.push_back((char)m_CharacterCurrent);

			found++;
		}

		if (found == 0)
		{
			_QueueCurrentCharacter();

			m_CharacterCurrent = previous;
		}

		return (found > 0);
	}

}; // namespace ExLibris