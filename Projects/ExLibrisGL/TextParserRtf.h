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
#include "RtfCharacterSet.h"
#include "RtfDomElement.h"
#include "RtfDomDocument.h"
#include "Token.h"

namespace ExLibris
{
	class RtfDomElement;
	class Tokenizer;
}

namespace ExLibris
{

	class TextParserRtf
		: public ITextParser
	{
	
	public:
	
		TextParserRtf();
		~TextParserRtf();

		RtfDomDocument* ParseDocument(std::basic_istream<char>* a_Stream);

		bool IsValid() const;

		void SetInput(const std::string& a_Text);

		bool ReadToken();

	private:

		enum ParseType
		{
			eParseType_Invalid,
			eParseType_GroupOpen,
			eParseType_GroupClose,
			eParseType_Command,
			eParseType_Value,
			eParseType_Text
		};

		struct RtfToken
		{
			RtfToken()
				: type(eParseType_Invalid)
				, index(-1)
			{
			}

			ParseType type;
			std::string value;
			int index;
		};

		typedef bool (TextParserRtf::*CommandHandler)(const RtfToken&);

		struct Group
		{
			Group()
				: parent(nullptr)
				, index(0)
				, process_commands(nullptr)
			{
			}

			Group* parent;
			int index;
			std::map<std::string, CommandHandler>* process_commands;
			CommandHandler process_value;
			std::vector<std::string> commands;
		};

	private:

		bool _ParseHeader();

		RtfToken _ReadNextToken();
		bool _ProcessToken(const RtfToken& a_Token);

	private:

		bool _CommandCharacterSet(const RtfToken& a_Token);
		bool _CommandFontTable(const RtfToken& a_Token);
		bool _CommandFont(const RtfToken& a_Token);
		bool _CommandFontFamily(const RtfToken& a_Token);
		bool _CommandParagraphResetToDefault(const RtfToken& a_Token);
		bool _CommandParagraph(const RtfToken& a_Token);

		bool _ProcessValueDefault(const RtfToken& a_Token);
		bool _ProcessValueFontEntry(const RtfToken& a_Token);

	private:

		RtfDomDocument* m_Document;
		RtfDomElement* m_ElementCurrent;

		std::stringstream m_Input;
		Tokenizer* m_Tokenizer;

		std::vector<Group*> m_Groups;
		Group* m_GroupCurrent;
		int m_GroupIndex;

		std::map<std::string, CommandHandler> m_CommandHandlers;
	
	}; // class TextParserRtf

}; // namespace ExLibris