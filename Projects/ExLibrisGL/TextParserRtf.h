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

#include "ITextParser.h"

namespace ExLibris
{
	class Tokenizer;
}

namespace ExLibris
{

	class TextParserRtf
		: public ITextParser
	{

	public:

		enum CharacterSet
		{
			eCharacterSet_Invalid,
			eCharacterSet_ANSI,
			eCharacterSet_AppleMacintosh,
			eCharacterSet_IBMPCCodePage437,
			eCharacterSet_IBMPCCodePage850,
		};
	
	public:
	
		TextParserRtf();
		~TextParserRtf();

		bool IsValid() const;

		CharacterSet GetCharacterSet() const;

		void SetInput(const std::string& a_Text);

		bool ReadToken();

	private:

		enum ParseType
		{
			eParseType_Invalid,
			eParseType_GroupOpen,
			eParseType_GroupClose,
			eParseType_Command
		};

		ParseType _Parse();

	private:

		std::stringstream m_Input;
		Tokenizer* m_Tokenizer;

		CharacterSet m_CharacterSet;
		bool m_Valid;

		std::string m_CommandCurrent;
		ParseType m_Parsed;
		int m_GroupIndex;
	
	}; // class TextParserRtf

}; // namespace ExLibris