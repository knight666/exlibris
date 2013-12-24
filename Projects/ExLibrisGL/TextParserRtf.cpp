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

#include "ExLibrisGL.PCH.h"

#include "TextParserRtf.h"

#include "Tokenizer.h"

namespace ExLibris
{

	TextParserRtf::TextParserRtf()
		: m_Tokenizer(nullptr)
		, m_Valid(false)
		, m_CharacterSet(eCharacterSet_ANSI)
		, m_GroupCurrent(nullptr)
		, m_GroupIndex(0)
		, m_FontEntryCurrent(nullptr)
	{
		m_Tokenizer = new Tokenizer(nullptr);
		m_Tokenizer->DisableOptions(Tokenizer::eOption_Identifiers);

		m_CommandHandlers.insert(std::make_pair("ansi", &TextParserRtf::_CommandCharacterSet));
		m_CommandHandlers.insert(std::make_pair("mac", &TextParserRtf::_CommandCharacterSet));
		m_CommandHandlers.insert(std::make_pair("pc", &TextParserRtf::_CommandCharacterSet));
		m_CommandHandlers.insert(std::make_pair("pca", &TextParserRtf::_CommandCharacterSet));
		m_CommandHandlers.insert(std::make_pair("fonttbl", &TextParserRtf::_CommandFontTable));

		m_CommandHandlersFontTable.insert(std::make_pair("f", &TextParserRtf::_CommandFont));
		m_CommandHandlersFontTable.insert(std::make_pair("fnil", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlersFontTable.insert(std::make_pair("froman", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlersFontTable.insert(std::make_pair("fswiss", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlersFontTable.insert(std::make_pair("fmodern", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlersFontTable.insert(std::make_pair("fscript", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlersFontTable.insert(std::make_pair("fdecor", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlersFontTable.insert(std::make_pair("ftech", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlersFontTable.insert(std::make_pair("fbidi", &TextParserRtf::_CommandFontFamily));
	}
	
	TextParserRtf::~TextParserRtf()
	{
		delete m_Tokenizer;

		for (std::vector<Group*>::iterator group_it = m_Groups.begin(); group_it != m_Groups.end(); ++group_it)
		{
			delete *group_it;
		}
		m_Groups.clear();
	}

	bool TextParserRtf::IsValid() const
	{
		return m_Valid;
	}

	TextParserRtf::CharacterSet TextParserRtf::GetCharacterSet() const
	{
		return m_CharacterSet;
	}

	void TextParserRtf::SetInput(const std::string& a_Text)
	{
		m_Input.clear();
		m_Input << a_Text;
		m_Tokenizer->SetInput(&m_Input);

		if (!_ParseHeader())
		{
			m_Valid = false;

			return;
		}

		// commands

		ParseType parsed = eParseType_Invalid;

		do 
		{
			parsed = _Parse();

			if (parsed == eParseType_Invalid)
			{
				m_Valid = false;
				return;
			}
		}
		while (parsed != eParseType_Text);

		m_Valid = true;
	}

	bool TextParserRtf::ReadToken()
	{
		return false;
	}

	bool TextParserRtf::_ReadCommand(const std::string& a_Text)
	{
		const Token& tk = m_Tokenizer->GetCurrentToken();

		if (!m_Tokenizer->ReadToken() || tk.type != Token::eType_Symbol || tk.text[0] != '\\')
		{
			return false;
		}

		if (!m_Tokenizer->ReadToken() || tk.type != Token::eType_Text || tk.text != a_Text)
		{
			return false;
		}

		return true;
	}

	int TextParserRtf::_ReadInteger()
	{
		const Token& tk = m_Tokenizer->GetCurrentToken();

		if (!m_Tokenizer->ReadToken() || tk.type != Token::eType_Integer)
		{
			return -1;
		}

		return atoi(tk.text.c_str());
	}

	bool TextParserRtf::_ParseHeader()
	{
		// document group

		if (_Parse() != eParseType_GroupOpen)
		{
			return false;
		}

		// rtf command

		if (!_ReadCommand("rtf") || _ReadInteger() != 1)
		{
			return false;
		}

		return true;
	}

	TextParserRtf::ParseType TextParserRtf::_Parse()
	{
		ParseType result = eParseType_Invalid;

		if (!m_Tokenizer->ReadToken())
		{
			return result;
		}

		const Token& tk = m_Tokenizer->GetCurrentToken();

		if (tk.type == Token::eType_Symbol)
		{
			switch (tk.text[0])
			{

			case '{':
				{
					m_GroupIndex++;

					Group* group_create = new Group;
					group_create->index = m_GroupIndex;
					group_create->parent = m_GroupCurrent;
					group_create->handlers = &m_CommandHandlers;

					m_GroupCurrent = group_create;

					m_Groups.push_back(group_create);

					result = eParseType_GroupOpen;

				} break;

			case '}':
				{
					m_GroupIndex--;

					if (m_GroupCurrent != nullptr && m_GroupCurrent->parent != nullptr)
					{
						m_GroupCurrent = m_GroupCurrent->parent;
					}

					result = eParseType_GroupClose;

				} break;

			case '\\':
				{
					if (!m_Tokenizer->ReadToken())
					{
						result = eParseType_Invalid;

						break;
					}

					if (tk.type == Token::eType_Text)
					{
						std::map<std::string, CommandHandler>::iterator found = m_GroupCurrent->handlers->find(tk.text);

						m_GroupCurrent->commands.push_back(tk.text);

						if (found != m_GroupCurrent->handlers->end())
						{
							CommandHandler handler = found->second;
							if (!(this->*handler)(tk))
							{
								std::cerr << "Error while parsing command \"\\" << tk.text << "\" at column " << tk.column << " on line " << tk.line << std::endl;
							}
						}
						else
						{
							std::cout << "Unhandled command \"\\" << tk.text << "\" at column " << tk.column << " on line " << tk.line << std::endl;
						}

						result = eParseType_Command;
					}
					
				} break;

			default:
				{
					result = eParseType_Text;

				} break;
			}
		}
		else if (tk.type == Token::eType_Whitespace || tk.type == Token::eType_Text)
		{
			result = eParseType_Text;
		}

		return result;
	}

	bool TextParserRtf::_CommandCharacterSet(const Token& a_Token)
	{
		if (m_CommandCurrent == "ansi")
		{
			m_CharacterSet = eCharacterSet_ANSI;
		}
		else if (m_CommandCurrent == "mac")
		{
			m_CharacterSet = eCharacterSet_AppleMacintosh;
		}
		else if (m_CommandCurrent == "pc")
		{
			m_CharacterSet = eCharacterSet_IBMPCCodePage437;
		}
		else if (m_CommandCurrent == "pca")
		{
			m_CharacterSet = eCharacterSet_IBMPCCodePage850;
		}

		return true;
	}

	bool TextParserRtf::_CommandFontTable(const Token& a_Token)
	{
		m_GroupCurrent->handlers = &m_CommandHandlersFontTable;

		return true;
	}

	bool TextParserRtf::_CommandFont(const Token& a_Token)
	{
		m_FontEntryCurrent = new FontEntry;
		m_FontEntryCurrent->index = _ReadInteger();

		m_FontEntries.insert(std::make_pair(m_FontEntryCurrent->index, m_FontEntryCurrent));

		/*const Token& tk = m_Tokenizer->GetCurrentToken();

		if (!m_Tokenizer->ReadToken() || tk.type != Token::eType_Whitespace || tk.text[0] != ' ')
		{
			return false;
		}

		while (m_Tokenizer->ReadToken())
		{
			if (tk.type == Token::eType_Symbol && tk.text[0] == ';')
			{
				break;
			}

			m_FontEntryCurrent->name += tk.text;
		}*/

		return (m_FontEntryCurrent->name.length() > 0);
	}

	bool TextParserRtf::_CommandFontFamily(const Token& a_Token)
	{
		if (m_FontEntryCurrent == nullptr)
		{
			std::cerr << "Could not set font family: no font entry was specified." << std::endl;

			return false;
		}

		if (a_Token.text == "froman")
		{
			m_FontEntryCurrent->family = eFamilyType_Roman;
		}
		else if (a_Token.text == "fswiss")
		{
			m_FontEntryCurrent->family = eFamilyType_Swiss;
		}
		else if (a_Token.text == "fmodern")
		{
			m_FontEntryCurrent->family = eFamilyType_Modern;
		}
		else if (a_Token.text == "fscript")
		{
			m_FontEntryCurrent->family = eFamilyType_Script;
		}
		else if (a_Token.text == "fdecor")
		{
			m_FontEntryCurrent->family = eFamilyType_Decor;
		}
		else if (a_Token.text == "ftech")
		{
			m_FontEntryCurrent->family = eFamilyType_Tech;
		}
		else if (a_Token.text == "fbidi")
		{
			m_FontEntryCurrent->family = eFamilyType_Bidi;
		}
		else
		{
			m_FontEntryCurrent->family = eFamilyType_Nil;
		}

		return true;
	}

}; // namespace ExLibris