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

#include "RtfColor.h"
#include "RtfDomElement.h"
#include "RtfTextFormat.h"
#include "Tokenizer.h"

namespace ExLibris
{

	class MessageFinisher
	{

	public:

		void operator = (RtfLogMessage& a_Message)
		{
			a_Message.Finish();
		}

	};

#define LOG_WARNING(_token) \
	::ExLibris::MessageFinisher() = \
	::ExLibris::RtfLogMessage(&m_LogWarnings, (_token).column, (_token).line)

#define LOG_ERROR(_token) \
	::ExLibris::MessageFinisher() = \
	::ExLibris::RtfLogMessage(&m_LogErrors, (_token).column, (_token).line)

	TextParserRtf::TextParserRtf()
		: m_Tokenizer(nullptr)
		, m_GroupCurrent(nullptr)
		, m_GroupIndex(0)
		, m_Document(nullptr)
		, m_ElementCurrent(nullptr)
	{
		m_Tokenizer = new Tokenizer(nullptr);
		m_Tokenizer->DisableOptions(Tokenizer::eOption_Identifiers);

		m_CommandHandlers.insert(std::make_pair("*", &TextParserRtf::_CommandExtended));

		m_CommandHandlers.insert(std::make_pair("ansi", &TextParserRtf::_CommandCharacterSet));
		m_CommandHandlers.insert(std::make_pair("mac", &TextParserRtf::_CommandCharacterSet));
		m_CommandHandlers.insert(std::make_pair("pc", &TextParserRtf::_CommandCharacterSet));
		m_CommandHandlers.insert(std::make_pair("pca", &TextParserRtf::_CommandCharacterSet));

		m_CommandHandlers.insert(std::make_pair("fonttbl", &TextParserRtf::_CommandFontTable));
		m_CommandHandlers.insert(std::make_pair("f", &TextParserRtf::_CommandFont));
		m_CommandHandlers.insert(std::make_pair("deff", &TextParserRtf::_CommandFontDefault));

		m_CommandHandlers.insert(std::make_pair("colortbl", &TextParserRtf::_CommandColorTable));

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

		m_LogWarnings.clear();
		m_LogErrors.clear();

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

	const std::vector<RtfLogMessage>& TextParserRtf::GetWarnings() const
	{
		return m_LogWarnings;
	}

	const std::vector<RtfLogMessage>& TextParserRtf::GetErrors() const
	{
		return m_LogErrors;
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
		if (header_command.type != eParseType_Command || header_command.value != "rtf" || header_command.parameter != 1)
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

		token.column = tk.column;
		token.line = tk.line;

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
					if (!m_Tokenizer->ReadToken())
					{
						token.type = eParseType_Invalid;

						return token;
					}

					token.type = eParseType_Command;

					if (tk.type == Token::eType_Text)
					{
						token.value = tk.text;

						if (m_Tokenizer->ReadToken())
						{
							if (tk.type == Token::eType_Integer)
							{
								token.parameter = atoi(tk.text.c_str());
							}
							else
							{
								m_Tokenizer->RevertToken();
							}
						}
					}
					else if (tk.type == Token::eType_Symbol && tk.text[0] == '*')
					{
						token.value = tk.text;
					}
					else
					{
						token.type = eParseType_Invalid;
					}

					// read trailing space

					if (m_Tokenizer->ReadToken() && (tk.type != Token::eType_Whitespace || tk.text[0] != ' '))
					{
						m_Tokenizer->RevertToken();
					}

					return token;

				} break;

			case ';':
				{
					token.type = eParseType_Value;

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

	void TextParserRtf::_GroupOpen()
	{
		m_GroupIndex++;

		Group* group_create = new Group;
		group_create->index = m_GroupIndex;
		group_create->parent = m_GroupCurrent;

		if (m_GroupCurrent != nullptr)
		{
			group_create->process_commands = m_GroupCurrent->process_commands;
			group_create->process_value = m_GroupCurrent->process_value;
		}
		else
		{
			group_create->process_commands = &m_CommandHandlers;
			group_create->process_value = &TextParserRtf::_ProcessValueDefault;
		}

		m_GroupCurrent = group_create;

		m_Groups.push_back(group_create);
	}

	void TextParserRtf::_GroupClose()
	{
		m_GroupIndex--;

		if (m_GroupCurrent != nullptr && m_GroupCurrent->parent != nullptr)
		{
			m_GroupCurrent = m_GroupCurrent->parent;
		}
	}

	bool TextParserRtf::_ProcessToken(const RtfToken& a_Token)
	{
		switch (a_Token.type)
		{

		case eParseType_GroupOpen:
			{
				_GroupOpen();

			} break;

		case eParseType_GroupClose:
			{
				_GroupClose();

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
						LOG_ERROR(a_Token) << "Error while parsing command \"\\" << a_Token.value << "\"";
					}
				}
				else
				{
					LOG_WARNING(a_Token) << "Unhandled command \"\\" << a_Token.value << "\"";
				}

			} break;

		case eParseType_Value:
			{
				CommandHandler handler = m_GroupCurrent->process_value;
				if (!(this->*handler)(a_Token))
				{
					LOG_ERROR(a_Token) << "Error while parsing value \"\\" << a_Token.value << "\"";
				}

			} break;

		case eParseType_Text:
			{
				m_ElementCurrent->InnerText += a_Token.value;

			} break;
		}

		return true;
	}


	RtfCharacterSet TextParserRtf::_TokenToCharset(const RtfToken& a_Token)
	{
		RtfCharacterSet character_set = eRtfCharacterSet_Invalid;

		if (a_Token.parameter >= 0)
		{

#define CHARSET_CASE(_index, _value) case _index: { character_set = _value; } break;

			switch (a_Token.parameter)
			{
				CHARSET_CASE(0, eRtfCharacterSet_Ansi);
				CHARSET_CASE(1, eRtfCharacterSet_Default);
				CHARSET_CASE(2, eRtfCharacterSet_Symbol);
				CHARSET_CASE(77, eRtfCharacterSet_AppleMacintoshRoman);
				CHARSET_CASE(78, eRtfCharacterSet_AppleMacintoshShiftJis);
				CHARSET_CASE(79, eRtfCharacterSet_AppleMacintoshHangul);
				CHARSET_CASE(80, eRtfCharacterSet_AppleMacintoshGb2332);
				CHARSET_CASE(81, eRtfCharacterSet_AppleMacintoshBig5);
				CHARSET_CASE(82, eRtfCharacterSet_AppleMacintoshJohabOld);
				CHARSET_CASE(83, eRtfCharacterSet_AppleMacintoshHebrew);
				CHARSET_CASE(84, eRtfCharacterSet_AppleMacintoshArabic);
				CHARSET_CASE(85, eRtfCharacterSet_AppleMacintoshGreek);
				CHARSET_CASE(86, eRtfCharacterSet_AppleMacintoshTurkish);
				CHARSET_CASE(87, eRtfCharacterSet_AppleMacintoshThai);
				CHARSET_CASE(88, eRtfCharacterSet_AppleMacintoshEasternEuropean);
				CHARSET_CASE(89, eRtfCharacterSet_AppleMacintoshRussian);
				CHARSET_CASE(128, eRtfCharacterSet_ShiftJis);
				CHARSET_CASE(129, eRtfCharacterSet_Hangul);
				CHARSET_CASE(130, eRtfCharacterSet_Johab);
				CHARSET_CASE(134, eRtfCharacterSet_Gb2332);
				CHARSET_CASE(136, eRtfCharacterSet_Big5);
				CHARSET_CASE(161, eRtfCharacterSet_Greek);
				CHARSET_CASE(162, eRtfCharacterSet_Turkish);
				CHARSET_CASE(163, eRtfCharacterSet_Vietnamese);
				CHARSET_CASE(177, eRtfCharacterSet_Hebrew);
				CHARSET_CASE(178, eRtfCharacterSet_Arabic);
				CHARSET_CASE(179, eRtfCharacterSet_ArabicTraditionalOld);
				CHARSET_CASE(180, eRtfCharacterSet_ArabicUserOld);
				CHARSET_CASE(181, eRtfCharacterSet_HebrewUserOld);
				CHARSET_CASE(186, eRtfCharacterSet_Baltic);
				CHARSET_CASE(204, eRtfCharacterSet_Russian);
				CHARSET_CASE(222, eRtfCharacterSet_Thai);
				CHARSET_CASE(238, eRtfCharacterSet_EasternEuropean);
				CHARSET_CASE(254, eRtfCharacterSet_IbmPcCodePage437);
				CHARSET_CASE(255, eRtfCharacterSet_Oem);
			}

#undef CHARSET_CASE

		}
		else if (a_Token.value.size() > 0)
		{
			if (a_Token.value == "ansi")
			{
				character_set = eRtfCharacterSet_Ansi;
			}
			else if (a_Token.value == "mac")
			{
				character_set = eRtfCharacterSet_AppleMacintoshRoman;
			}
			else if (a_Token.value == "pc")
			{
				character_set = eRtfCharacterSet_IbmPcCodePage437;
			}
			else if (a_Token.value == "pca")
			{
				character_set = eRtfCharacterSet_Oem;
			}
		}
		
		return character_set;
	}

	bool TextParserRtf::_CommandExtended(const RtfToken& a_Token)
	{
		Group* group_previous = m_GroupCurrent->parent;
		if (group_previous == nullptr)
		{
			LOG_ERROR(a_Token) << "Extended controls must be contained in their own group.";

			return false;
		}

		RtfToken token = _ReadNextToken();

		if (token.type != eParseType_Command)
		{
			LOG_ERROR(a_Token) << "Extended control must be followed by another control.";

			return false;
		}

		// check if command is known

		std::map<std::string, CommandHandler>::iterator found = m_GroupCurrent->process_commands->find(token.value);

		if (found != m_GroupCurrent->process_commands->end())
		{
			return _ProcessToken(token);
		}

		// skip entire group

		while (token.type != eParseType_Invalid)
		{
			switch (token.type)
			{

			case eParseType_GroupOpen:
				{
					_GroupOpen();

				} break;

			case eParseType_GroupClose:
				{
					_GroupClose();

					if (m_GroupCurrent == group_previous)
					{
						return true;
					}

				} break;
			}

			token = _ReadNextToken();
		}

		return true;
	}

	bool TextParserRtf::_CommandCharacterSet(const RtfToken& a_Token)
	{
		m_ElementCurrent->TextFormat.character_set = _TokenToCharset(a_Token);

		return (m_ElementCurrent->TextFormat.character_set != eRtfCharacterSet_Invalid);
	}

	bool TextParserRtf::_CommandFontTable(const RtfToken& a_Token)
	{
		Group* group_fonttable_parent = m_GroupCurrent->parent;
		if (group_fonttable_parent == nullptr)
		{
			LOG_ERROR(a_Token) << "Font table must be inside a group.";

			return false;
		}

		RtfFontTable* font_table = m_Document->GetFontTable();
		RtfFont* font = nullptr;

		RtfToken token = _ReadNextToken();
		while (token.type != eParseType_Invalid)
		{
			switch (token.type)
			{

			case eParseType_GroupOpen:
				{
					_GroupOpen();

				} break;

			case eParseType_GroupClose:
				{
					_GroupClose();

					if (m_GroupCurrent == group_fonttable_parent)
					{
						return true;
					}

				} break;

			case eParseType_Command:
				{
					if (token.value == "f")
					{
						if (token.parameter < 0)
						{
							LOG_ERROR(a_Token) << "Invalid parameter \"" << token.parameter << "\" for font control.";

							return false;
						}

						font = font_table->GetFont(token.parameter);
					}
					else
					{
						if (font == nullptr)
						{
							LOG_ERROR(a_Token) << "Invalid control \\" << token.value << " because target font was not set.";

							return false;
						}

						// family

						if (token.value == "froman")
						{
							font->family = RtfFont::eFamilyType_Roman;
						}
						else if (token.value == "fswiss")
						{
							font->family = RtfFont::eFamilyType_Swiss;
						}
						else if (token.value == "fmodern")
						{
							font->family = RtfFont::eFamilyType_Modern;
						}
						else if (token.value == "fscript")
						{
							font->family = RtfFont::eFamilyType_Script;
						}
						else if (token.value == "fdecor")
						{
							font->family = RtfFont::eFamilyType_Decor;
						}
						else if (token.value == "ftech")
						{
							font->family = RtfFont::eFamilyType_Tech;
						}
						else if (token.value == "fbidi")
						{
							font->family = RtfFont::eFamilyType_Bidi;
						}

						// pitch

						else if (token.value == "fprq")
						{
							if (token.parameter < RtfFont::ePitch_Default || token.parameter > RtfFont::ePitch_Variable)
							{
								LOG_ERROR(a_Token) << "Invalid pitch (" << token.parameter << ") specified for font.";

								return false;
							}

							font->pitch = (RtfFont::Pitch)token.parameter;
						}

						// characterset

						else if (token.value == "fcharset")
						{
							font->character_set = _TokenToCharset(token);

							if (font->character_set == eRtfCharacterSet_Invalid)
							{
								LOG_ERROR(a_Token) << "Invalid character set (" << token.parameter << ") specified for font.";

								return false;
							}
						}

						// unhandled

						else
						{
							LOG_WARNING(a_Token) << "Unhandled control \"\\" << token.value << "\"";
						}
					}

				} break;

			case eParseType_Value:
				{
					font->name = token.value;

				} break;

			}

			token = _ReadNextToken();
		}

		return true;
	}

	bool TextParserRtf::_CommandFont(const RtfToken& a_Token)
	{
		if (a_Token.parameter == -1)
		{
			return false;
		}

		m_ElementCurrent->TextFormat.font = m_Document->GetFontTable()->GetFont(a_Token.parameter);

		return true;
	}

	bool TextParserRtf::_CommandFontDefault(const RtfToken& a_Token)
	{
		if (a_Token.parameter == -1)
		{
			return false;
		}

		m_Document->GetFontTable()->SetDefault(a_Token.parameter);

		if (m_ElementCurrent->TextFormat.font == nullptr)
		{
			m_ElementCurrent->TextFormat.font = m_Document->GetFontTable()->GetDefault();
		}

		return true;
	}

	bool TextParserRtf::_CommandColorTable(const RtfToken& a_Token)
	{
		RtfToken token = _ReadNextToken();

		// first empty value indicator

		if (token.type != eParseType_Value)
		{
			return false;
		}

		RtfColorTable* color_table = m_Document->GetColorTable();
		RtfColor* color = color_table->AddColor(*color_table->GetDefaultColor());

		token = _ReadNextToken();

		while (token.type != eParseType_Invalid)
		{
			if (token.type == eParseType_Command)
			{
				if (token.value == "red")
				{
					color->r = token.parameter;
				}
				else if (token.value == "green")
				{
					color->g = token.parameter;
				}
				else if (token.value == "blue")
				{
					color->b = token.parameter;
				}
				else
				{
					LOG_ERROR(a_Token) << "Unknown color control \\" << token.parameter << "\\";
				}
			}
			else if (token.type == eParseType_Value)
			{
				color = color_table->AddColor(*color_table->GetDefaultColor());
			}
			else
			{
				m_Tokenizer->RevertToken();

				break;
			}

			token = _ReadNextToken();
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

}; // namespace ExLibris