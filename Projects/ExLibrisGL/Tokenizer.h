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

		bool _IsNextCharacterAvailable() const;
		void _NextCharacter();
		void _AddCurrentToToken();
		void _QueueCurrentCharacter();
		void _UndoConsomed();
		
		template<typename CharacterType>
		bool _IsCharacterOfType(int a_Character);

		Token::Type _GetTypeForCharacter(int a_Character);

		template<typename CharacterType>
		bool _TryConsumeOne();

		template<typename CharacterType>
		bool _TryConsumeOneOrMore();

		bool _TryConsume(int a_Character);

		bool _ConsumeNumber();
		bool _ConsumeNumberInteger();
		bool _ConsumeNumberOctal();
		bool _ConsumeString();
		bool _ConsumeSymbol();
		bool _ConsumeWhitespace();
		bool _ConsumeNewLine();
		void _ConsumeText();

	private:

		std::basic_istream<char>* m_Stream;

		Token m_TokenCurrent;

		int m_CharacterCurrent;
		std::deque<int> m_CharacterQueue;

		std::deque<int> m_CharactersUndoConsumed;

		int m_Column;
		int m_Line;
	
	}; // class Tokenizer

	template<typename CharacterType>
	inline bool Tokenizer::_IsCharacterOfType(int a_Character)
	{
		return (CharacterType::IsKnown(a_Character));
	}

	template<typename CharacterType>
	inline bool Tokenizer::_TryConsumeOne()
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

	template<typename CharacterType>
	inline bool Tokenizer::_TryConsumeOneOrMore()
	{
		if (!CharacterType::IsKnown(m_CharacterCurrent))
		{
			return false;
		}

		_AddCurrentToToken();

		while (_IsNextCharacterAvailable())
		{
			_NextCharacter();

			if (!CharacterType::IsKnown(m_CharacterCurrent))
			{
				_QueueCurrentCharacter();

				break;
			}

			_AddCurrentToToken();
		}

		return true;
	}

}; // namespace ExLibris