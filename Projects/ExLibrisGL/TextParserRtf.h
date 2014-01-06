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

#include "Token.h"

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

		RtfToken _ReadNextToken();
		bool _ProcessToken(const RtfToken& a_Token);

		bool _ReadCommand(const std::string& a_Text);
		int _ReadInteger();
		bool _ParseHeader();
		ParseType _Parse();
		bool _ParseCommand(const Token& a_Token);

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

		enum FamilyType
		{
			eFamilyType_Nil,
			eFamilyType_Roman,
			eFamilyType_Swiss,
			eFamilyType_Modern,
			eFamilyType_Script,
			eFamilyType_Decor,
			eFamilyType_Tech,
			eFamilyType_Bidi,
		};

		struct FontEntry
		{
			FontEntry()
				: index(0)
				, family(eFamilyType_Nil)
			{
			}

			int index;
			FamilyType family;
			std::string name;
		};

	private:

		std::stringstream m_Input;
		Tokenizer* m_Tokenizer;

		CharacterSet m_CharacterSet;
		bool m_Valid;

		std::string m_CommandCurrent;
		ParseType m_Parsed;
		int m_GroupIndex;

		std::vector<Group*> m_Groups;
		Group* m_GroupCurrent;

		std::map<int, FontEntry*> m_FontEntries;
		FontEntry* m_FontEntryCurrent;

	private:

		bool _CommandCharacterSet(const RtfToken& a_Token);
		bool _CommandFontTable(const RtfToken& a_Token);
		bool _CommandUseFont(const RtfToken& a_Token);
		bool _CommandFontDefinition(const RtfToken& a_Token);
		bool _CommandFontFamily(const RtfToken& a_Token);
		bool _CommandParagraphResetToDefault(const RtfToken& a_Token);
		bool _CommandParagraphEnd(const RtfToken& a_Token);

		bool _ProcessValueDefault(const RtfToken& a_Token);
		bool _ProcessValueFontEntry(const RtfToken& a_Token);

		std::map<std::string, CommandHandler> m_CommandHandlers;
		std::map<std::string, CommandHandler> m_CommandHandlersFontTable;
	
	}; // class TextParserRtf

}; // namespace ExLibris