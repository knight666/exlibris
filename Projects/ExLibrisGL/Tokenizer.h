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

	/*!
		\brief Converts text input into tokens.

		This class reads input from a stream and converts it to Token instances.
		Tokens are used for parsing programming languages and other text-based input.

		For example, let's say a JSON file is being parsed and the following line is encountered:

			"fuel": 15.9,
		
		This input will generate five tokens:

			""fuel"" of type "string" at line 1 and column 1
			":" of type "symbol" at line 1 and column 6
			" " of type "whitespace" at line 1 and column 7
			"15.9" of type "number" at line 1 and column 8
			"," of type "symbol" at line 1 and column 12
			"" of type "end" at line 1 and column 13

		The JSON parser then acts on these tokens, without having to validate them beforehand.

		Tokenization is a greedy process: the tokenizer will consume as much input as possible for a given token.
		For example, the input "item32" results in one identifier token, instead of a text token and an integer token.

		Note that the tokenizer can only read forwards. If an invalid token is encountered,
		the input is rolled back and treated as valid tokens of a different type.

		Use-case
		=======

		The tokenizer is intended to be used as follows:

			Tokenizer tk;
			tk.EnableOptions(my_enabled_options);
			tk.DisableOptions(my_disabled_options);
			tk.SetInput(my_input);

			while (tk.ReadToken())
			{
				Process(tk.GetCurrentToken());
			}

		Note that options can be changed during processing.
	*/
	class Tokenizer
	{

	public:

		//! Options for the tokenization.

		enum Option
		{
			eOption_Identifiers = (1 << 0) /**< Allow identifiers in the output. If disabled, text tokens are generated instead. */
		};
	
	public:
	
		Tokenizer(std::basic_istream<char>* a_Stream);
		~Tokenizer();
		
		/*!
			\brief Check if an option is enabled

			\param a_Option The option to check.
			\return True when enabled, false when disabled.
			\sa IsOptionDisabled, EnableOptions
		*/
		bool IsOptionEnabled(Option a_Option) const;

		/*!
			\brief Enable one or more options

			\param a_Options The bitwise-or of one or more options.
			\sa DisableOptions, IsOptionEnabled
		*/
		void EnableOptions(unsigned int a_Options);

		/*!
			\brief Check if an option is disabled

			\param a_Option The option to check.
			\return True when disabled, false when enabled.
			\sa IsOptionEnabled, DisableOptions
		*/
		bool IsOptionDisabled(Option a_Option) const;

		/*!
			\brief Disable one or more options

			\param a_Options The bitwise-or of one or more options.
			\sa EnableOptinos, IsOptionDisabled
		*/
		void DisableOptions(unsigned int a_Options);

		/*!
			\brief Sets the width of a tab character

			The tab width denotes the *maximum* width in spaces of a tab character. Calculating
			the width of a tab character is an involved process, because it depends on the
			position of the character on the line. 

			An invalid tab width will set the tab width to 1.

			\param a_Width A width in spaces.
		*/
		void SetTabWidth(size_t a_Width);

		/*!
			\brief Set the input used for generating tokens

			The input stream is used for reading characters to convert into tokens. If the input
			is invalid, only an end token is available. The input is only read forwards, never
			backwards.

			\param a_Stream Input stream used for reading characters.
		*/
		void SetInput(std::basic_istream<char>* a_Stream);

		/*!
			\brief Get the current token

			The current token is the one read most recently by ReadToken. No other tokens are
			available.

			\return The current token.
			\sa ReadToken
		*/
		const Token& GetCurrentToken() const;

		/*!
			\brief Check if the next token is available

			\return True if another token is available, false if not
		*/
		bool IsNextTokenAvailable() const;

		/*!
			\brief Reads a token from the input

			A valid token is read from the input. The last token read
			is always an end token and will return false.

			\return True on success, false on failure
			\sa IsNextTokenAvailable, GetCurrentToken
		*/
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

		inline bool _Consume(int a_Character)
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

		inline bool _ConsumeEither(int a_A, int a_B)
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
		inline bool _ConsumeType()
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