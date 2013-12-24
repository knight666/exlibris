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
	/*!
		\brief Container for token information.

		A token is output from the Tokenizer. It has a type, a value, a column and a row.
		This information is used for parsing programming languages and other text-based input.

		Tokens are not intended to be constructed without a Tokenizer, but can be copied.

		Unprintable
		==============

		Unprintable or control characters affect the output, but are not rendered themselves. Examples include
		the tab character `\t`, the newline character `\n` and the linefeed character `\r`.

		Every character between ASCII value 0 and 31 is classified or reserved as a control character.

		Identifier
		==============

		Identifier tokens consist of at least one alphanumeric character and cannot start with a digit. They are
		allowed to start and end with one or more underscores, but not dashes.

		Can be enabled or disabled using Tokenizer::eOption_Identifiers.

		Text
		==============

		Text tokens contain everything not captured by other token types.

		String
		==============

		String literals are special text tokens that start and end with a quotation symbol. They are not
		allowed to span multiple lines. Quote symbols found after opening a string token are not considered
		new string tokens.

		Can be enabled or disabled using Tokenizer::eOption_Strings.

		Whitespace
		=============

		Whitespace comes in the form of space and tab characters. Each space will advance the column position
		by one, while a tab will advance the column position by a value determined by the tab width and current
		column position.

		Leading whitespace can be ignored using Tokenizer::eOption_IgnoreLeadingWhitespace.

		New line
		=============

		New line tokens advance the line position by one. They can consist of `\r`, `\n` or `\r\n` depending
		on the settings.

		New line settings can be set using Tokenizer::SetNewLinePolicy.

		Symbol
		==============

		Symbol tokens always hold a single character and consist of printable characters that are neither
		text nor digits, for example `(` and `@`.

		Integer
		==============

		A base-10 integer token consists of one or more digits between 0 and 9. An integer starting with a 0
		can become an octal token, while an integer split using a dot character can become a number token.

		Can be enabled or disabled using Tokenizer::eOption_NumbersOnly.

		Octal
		==============

		A base-8 integer that starts with 0, consisting of digits between 0 and 7. A zero followed by an `x`
		character is interpreted as a hexadecimal token.

		Can be enabled or disabled using Tokenizer::eOption_NumbersOnly.

		Hexadecimal
		==============

		A base-16 integer that starts with `0x`, consisting of digits between 0 and 9 and letters between A and F.

		Can be enabled or disabled using Tokenizer::eOption_NumbersOnly.

		Number
		==============

		An integer separated by a single dot (`.`) character. Numbers divided by an `e` or `E` character can
		become a scientific token.

		Scientific
		==============

		Shorthand for a number with a lot of zero's, either a very small or a very large value. Valid
		scientific tokens have a positive or negative power value.

		Example:

			1e3 becomes 1000
			2.4e-4 becomes 0.00024

		Can be enabled or disabled using Tokenizer::eOption_NumbersOnly.

		End
		==============

		Token denoting the end of (valid) input.
	*/
	struct Token
	{

	public:

		//! Types available for tokens

		enum Type
		{
			eType_Unprintable,   /**< Unprintable or control characters */
			eType_Identifier,    /**< Alphanumeric characters */
			eType_Text,          /**< Default */
			eType_String,        /**< Text in quotation marks */
			eType_Whitespace,    /**< Space or tab characters */
			eType_NewLine,       /**< New line */
			eType_Symbol,        /**< Printable, neither text nor digit */
			eType_Integer,       /**< Base 10 integer, digits 0-9 */
			eType_Octal,         /**< Base 8 integer, digits 0-7 */
			eType_Hexadecimal,   /**< Base 16 integer, digits 0-9 and letters A-F */
			eType_Number,        /**< Integer split by a floating point */
			eType_Scientific,    /**< Shorthand for long floating point numbers */
			eType_End            /**< End of input */
		};

	public:

		Token()
			: type(eType_End)
			, column(0)
			, line(0)
		{
		}

		Type type;
		std::string text;
		int column;
		int line;

	}; // struct Token

}; // namespace ExLibris