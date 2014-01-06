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

#include "RtfDomElement.h"
#include "RtfTextFormat.h"
#include "Tokenizer.h"

namespace ExLibris
{

	TextParserRtf::TextParserRtf()
		: m_Tokenizer(nullptr)
		, m_Document(nullptr)
		, m_ElementCurrent(nullptr)
		, m_GroupCurrent(nullptr)
		, m_GroupIndex(0)
	{
		m_Tokenizer = new Tokenizer(nullptr);
		m_Tokenizer->DisableOptions(Tokenizer::eOption_Identifiers);

		m_CommandHandlers.insert(std::make_pair("ansi", &TextParserRtf::_CommandCharacterSet));
		m_CommandHandlers.insert(std::make_pair("mac", &TextParserRtf::_CommandCharacterSet));
		m_CommandHandlers.insert(std::make_pair("pc", &TextParserRtf::_CommandCharacterSet));
		m_CommandHandlers.insert(std::make_pair("pca", &TextParserRtf::_CommandCharacterSet));

		m_CommandHandlers.insert(std::make_pair("fonttbl", &TextParserRtf::_CommandFontTable));
		m_CommandHandlers.insert(std::make_pair("f", &TextParserRtf::_CommandFont));
		m_CommandHandlers.insert(std::make_pair("fnil", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlers.insert(std::make_pair("froman", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlers.insert(std::make_pair("fswiss", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlers.insert(std::make_pair("fmodern", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlers.insert(std::make_pair("fscript", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlers.insert(std::make_pair("fdecor", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlers.insert(std::make_pair("ftech", &TextParserRtf::_CommandFontFamily));
		m_CommandHandlers.insert(std::make_pair("fbidi", &TextParserRtf::_CommandFontFamily));

		m_CommandHandlers.insert(std::make_pair("pard", &TextParserRtf::_CommandParagraphResetToDefault));
		m_CommandHandlers.insert(std::make_pair("par", &TextParserRtf::_CommandParagraph));
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

	RtfDomDocument* TextParserRtf::ParseDocument(std::basic_istream<char>* a_Stream)
	{
		if (a_Stream == nullptr)
		{
			return nullptr;
		}

		m_Document = new RtfDomDocument;

		m_Tokenizer->SetInput(a_Stream);

		if (!_ParseHeader())
		{
			delete m_Document;
			m_Document = nullptr;

			return nullptr;
		}

		m_ElementCurrent = m_Document->GetRootElement();

		RtfToken command = _ReadNextToken();
		while (command.type != eParseType_Invalid)
		{
			if (!_ProcessToken(command))
			{
				break;
			}

			command = _ReadNextToken();
		}

		return m_Document;
	}

	bool TextParserRtf::IsValid() const
	{
		return false;
	}

	void TextParserRtf::SetInput(const std::string& a_Text)
	{
	}

	bool TextParserRtf::ReadToken()
	{
		return false;
	}

	bool TextParserRtf::_ParseHeader()
	{
		// document group

		RtfToken document_group = _ReadNextToken();
		if (document_group.type != eParseType_GroupOpen)
		{
			return false;
		}

		m_GroupIndex = 0;

		m_GroupCurrent = new Group;
		m_GroupCurrent->index = m_GroupIndex++;
		m_GroupCurrent->parent = nullptr;
		m_GroupCurrent->process_commands = &m_CommandHandlers;
		m_GroupCurrent->process_value = &TextParserRtf::_ProcessValueDefault;

		m_Groups.push_back(m_GroupCurrent);

		// rtf command

		RtfToken header_command = _ReadNextToken();
		if (header_command.type != eParseType_Command || header_command.value != "rtf" || header_command.index != 1)
		{
			return false;
		}

		return true;
	}

	TextParserRtf::RtfToken TextParserRtf::_ReadNextToken()
	{
		RtfToken token;
		token.type = eParseType_Invalid;

		if (!m_Tokenizer->ReadToken())
		{
			return token;
		}

		const Token& tk = m_Tokenizer->GetCurrentToken();

		if (tk.type == Token::eType_Symbol)
		{
			switch (tk.text[0])
			{

			case '{':
				{
					token.type = eParseType_GroupOpen;

				} break;

			case '}':
				{
					token.type = eParseType_GroupClose;

				} break;

			case '\\':
				{
					token.type = eParseType_Command;

					if (m_Tokenizer->ReadToken() && tk.type == Token::eType_Text)
					{
						token.value = tk.text;

						if (m_Tokenizer->ReadToken())
						{
							if (tk.type == Token::eType_Integer)
							{
								token.index = atoi(tk.text.c_str());
							}
							else
							{
								m_Tokenizer->RevertToken();
							}
						}

						// read trailing space

						if (m_Tokenizer->ReadToken() && (tk.type != Token::eType_Whitespace || tk.text[0] != ' '))
						{
							m_Tokenizer->RevertToken();
						}
					}
					else
					{
						token.type = eParseType_Invalid;
					}

					return token;

				} break;

			default:
				{
					token.value += tk.text;
					token.type = eParseType_Text;

				} break;
			}
		}
		else if (tk.type == Token::eType_NewLine)
		{
			while (tk.type == Token::eType_NewLine)
			{
				if (!m_Tokenizer->ReadToken())
				{
					token.type = eParseType_Invalid;

					return token;
				}
			}
		}

		if (tk.type == Token::eType_Text || tk.type == Token::eType_Whitespace)
		{
			token.type = eParseType_Text;
			token.value = tk.text;

			while (m_Tokenizer->ReadToken() && (tk.type == Token::eType_Text || tk.type == Token::eType_Whitespace || tk.type == Token::eType_Symbol))
			{
				if (tk.type == Token::eType_Symbol)
				{
					char symbol = tk.text[0];

					if (symbol == '{' || symbol == '}' || symbol == '\\')
					{
						m_Tokenizer->RevertToken();

						break;
					}
					else if (symbol == ';')
					{
						token.type = eParseType_Value;

						break;
					}
				}

				token.value += tk.text;
			}
		}

		return token;
	}

	bool TextParserRtf::_ProcessToken(const RtfToken& a_Token)
	{
		switch (a_Token.type)
		{

		case eParseType_GroupOpen:
			{
				m_GroupIndex++;

				Group* group_create = new Group;
				group_create->index = m_GroupIndex;
				group_create->parent = m_GroupCurrent;
				group_create->process_commands = &m_CommandHandlers;
				group_create->process_value = &TextParserRtf::_ProcessValueDefault;

				m_GroupCurrent = group_create;

				m_Groups.push_back(group_create);

			} break;

		case eParseType_GroupClose:
			{
				m_GroupIndex--;

				if (m_GroupCurrent != nullptr && m_GroupCurrent->parent != nullptr)
				{
					m_GroupCurrent = m_GroupCurrent->parent;
				}

			} break;

		case eParseType_Command:
			{
				std::map<std::string, CommandHandler>::iterator found = m_GroupCurrent->process_commands->find(a_Token.value);

				m_GroupCurrent->commands.push_back(a_Token.value);

				if (found != m_GroupCurrent->process_commands->end())
				{
					CommandHandler handler = found->second;
					if (!(this->*handler)(a_Token))
					{
						std::cerr << "Error while parsing command \"\\" << a_Token.value << "\"" << std::endl;
					}
				}
				else
				{
					std::cout << "Unhandled command \"\\" << a_Token.value << "\"" << std::endl;
				}

			} break;

		case eParseType_Value:
			{
				CommandHandler handler = m_GroupCurrent->process_value;
				if (!(this->*handler)(a_Token))
				{
					std::cerr << "Error while parsing value \"\\" << a_Token.value << "\"" << std::endl;
				}

			} break;

		case eParseType_Text:
			{
				m_ElementCurrent->InnerText += a_Token.value;

			} break;
		}

		return true;
	}

	bool TextParserRtf::_CommandCharacterSet(const RtfToken& a_Token)
	{
		RtfTextFormat& format = m_ElementCurrent->TextFormat;

		if (a_Token.value == "ansi")
		{
			format.character_set = eRtfCharacterSet_Ansi;
		}
		else if (a_Token.value == "mac")
		{
			format.character_set = eRtfCharacterSet_AppleMacintosh;
		}
		else if (a_Token.value == "pc")
		{
			format.character_set = eRtfCharacterSet_IbmPcCodePage437;
		}
		else if (a_Token.value == "pca")
		{
			format.character_set = eRtfCharacterSet_IbmPcCodePage850;
		}

		return true;
	}

	bool TextParserRtf::_CommandFontTable(const RtfToken& a_Token)
	{
		m_GroupCurrent->process_value = &TextParserRtf::_ProcessValueFontEntry;

		return true;
	}

	bool TextParserRtf::_CommandFont(const RtfToken& a_Token)
	{
		if (a_Token.index == -1)
		{
			return false;
		}

		m_ElementCurrent->TextFormat.font = &m_Document->GetFont(a_Token.index);

		return true;
	}

	bool TextParserRtf::_CommandFontFamily(const RtfToken& a_Token)
	{
		RtfFont* font = m_ElementCurrent->TextFormat.font;

		if (font == nullptr)
		{
			std::cerr << "Could not set font family: no font entry was specified." << std::endl;

			return false;
		}

		if (a_Token.value == "froman")
		{
			font->family = RtfFont::eFamilyType_Roman;
		}
		else if (a_Token.value == "fswiss")
		{
			font->family = RtfFont::eFamilyType_Swiss;
		}
		else if (a_Token.value == "fmodern")
		{
			font->family = RtfFont::eFamilyType_Modern;
		}
		else if (a_Token.value == "fscript")
		{
			font->family = RtfFont::eFamilyType_Script;
		}
		else if (a_Token.value == "fdecor")
		{
			font->family = RtfFont::eFamilyType_Decor;
		}
		else if (a_Token.value == "ftech")
		{
			font->family = RtfFont::eFamilyType_Tech;
		}
		else if (a_Token.value == "fbidi")
		{
			font->family = RtfFont::eFamilyType_Bidi;
		}
		else
		{
			font->family = RtfFont::eFamilyType_Nil;
		}

		return true;
	}

	bool TextParserRtf::_CommandParagraphResetToDefault(const RtfToken& a_Token)
	{
		return true;
	}

	bool TextParserRtf::_CommandParagraph(const RtfToken& a_Token)
	{
		m_ElementCurrent = m_ElementCurrent->AddChild();

		return true;
	}

	bool TextParserRtf::_ProcessValueDefault(const RtfToken& a_Token)
	{
		return true;
	}

	bool TextParserRtf::_ProcessValueFontEntry(const RtfToken& a_Token)
	{
		if (m_ElementCurrent->TextFormat.font == nullptr)
		{
			std::cerr << "Could not set font name: no font entry was specified." << std::endl;

			return false;
		}

		m_ElementCurrent->TextFormat.font->name = a_Token.value;

		return true;
	}

}; // namespace ExLibris