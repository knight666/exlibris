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

#include "TextParserToken.h"

namespace ExLibris
{

	class TextParserMarkdown
	{

	public:
	
		TextParserMarkdown();
		~TextParserMarkdown();

		void SetInput(const std::string& a_Text);

		bool ReadToken();

		const TextParserToken& GetToken() const;
	
	private:

		int _NextCodepoint();
		bool _ReadNextCodepoint();
		void _ParseNewline();
		void _ToggleItalic();
		void _ToggleBold();

	private:

		std::string m_Text;
		std::string::iterator m_TextCursor;
		int m_CodepointCurrent;
		TextParserToken m_Token;

		bool m_EscapeCharacter;
	
	}; // class TextParserMarkdown

}; // namespace ExLibris