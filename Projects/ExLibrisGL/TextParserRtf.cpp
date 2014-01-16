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
#include "RtfWorld.h"
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
		, m_World(new RtfWorld())
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

		m_CommandHandlers.insert(std::make_pair("stylesheet", &TextParserRtf::_CommandStyleSheet));

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

		delete m_World;
	}

	RtfDomDocument* TextParserRtf::ParseDocument(std::basic_istream<char>* a_Stream)
	{
		if (a_Stream == nullptr)
		{
			return nullptr;
		}

		m_LogWarnings.clear();
		m_LogErrors.clear();

		m_Document = new RtfDomDocument(m_World);

		m_Tokenizer->SetInput(a_Stream);

		if (!_ParseHeader())
		{
			delete m_Document;
			m_Document = nullptr;

			return nullptr;
		}

		m_ElementCurrent = m_Document->GetRootElement();

		while (_ReadNextToken() && m_TokenCurrent.type != RtfToken::eParseType_Invalid)
		{
			if (!_ProcessToken(m_TokenCurrent))
			{
				break;
			}
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

		_ReadNextToken();
		if (m_TokenCurrent.type != RtfToken::eParseType_GroupOpen)
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

		_ReadNextToken();
		if (m_TokenCurrent.type != RtfToken::eParseType_Command || m_TokenCurrent.value != "rtf" || m_TokenCurrent.parameter != 1)
		{
			return false;
		}

		return true;
	}

	bool TextParserRtf::_ReadNextToken()
	{
		m_TokenCurrent.type = RtfToken::eParseType_Invalid;
		m_TokenCurrent.parameter = -1;
		m_TokenCurrent.column = 0;
		m_TokenCurrent.line = 0;

		if (!m_Tokenizer->ReadToken())
		{
			return false;
		}

		const Token& tk = m_Tokenizer->GetCurrentToken();

		m_TokenCurrent.column = tk.column;
		m_TokenCurrent.line = tk.line;

		if (tk.type == Token::eType_Symbol)
		{
			switch (tk.text[0])
			{

			case '{':
				{
					m_TokenCurrent.type = RtfToken::eParseType_GroupOpen;

				} break;

			case '}':
				{
					m_TokenCurrent.type = RtfToken::eParseType_GroupClose;

				} break;

			case '\\':
				{
					if (!m_Tokenizer->ReadToken())
					{
						m_TokenCurrent.type = RtfToken::eParseType_Invalid;

						return false;
					}

					m_TokenCurrent.type = RtfToken::eParseType_Command;

					if (tk.type == Token::eType_Text)
					{
						m_TokenCurrent.value = tk.text;

						if (m_Tokenizer->ReadToken())
						{
							if (tk.type == Token::eType_Integer)
							{
								m_TokenCurrent.parameter = atoi(tk.text.c_str());
							}
							else
							{
								m_Tokenizer->RevertToken();
							}
						}
					}
					else if (tk.type == Token::eType_Symbol && tk.text[0] == '*')
					{
						m_TokenCurrent.value = tk.text;
					}
					else
					{
						m_TokenCurrent.type = RtfToken::eParseType_Invalid;
					}

					// read trailing space

					if (m_Tokenizer->ReadToken() && (tk.type != Token::eType_Whitespace || tk.text[0] != ' '))
					{
						m_Tokenizer->RevertToken();
					}

					return true;

				} break;

			case ';':
				{
					m_TokenCurrent.type = RtfToken::eParseType_Value;

				} break;

			default:
				{
					m_TokenCurrent.value += tk.text;
					m_TokenCurrent.type = RtfToken::eParseType_Text;

				} break;
			}
		}
		else if (tk.type == Token::eType_NewLine)
		{
			while (tk.type == Token::eType_NewLine)
			{
				if (!m_Tokenizer->ReadToken())
				{
					m_TokenCurrent.type = RtfToken::eParseType_Invalid;

					return false;
				}
			}
		}

		if (tk.type == Token::eType_Text || tk.type == Token::eType_Whitespace)
		{
			m_TokenCurrent.type = RtfToken::eParseType_Text;
			m_TokenCurrent.value = tk.text;

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
						m_TokenCurrent.type = RtfToken::eParseType_Value;

						break;
					}
				}

				m_TokenCurrent.value += tk.text;
			}
		}

		return true;
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

		case RtfToken::eParseType_GroupOpen:
			{
				_GroupOpen();

			} break;

		case RtfToken::eParseType_GroupClose:
			{
				_GroupClose();

			} break;

		case RtfToken::eParseType_Command:
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

		case RtfToken::eParseType_Value:
			{
				CommandHandler handler = m_GroupCurrent->process_value;
				if (!(this->*handler)(a_Token))
				{
					LOG_ERROR(a_Token) << "Error while parsing value \"\\" << a_Token.value << "\"";
				}

			} break;

		case RtfToken::eParseType_Text:
			{
				m_ElementCurrent->InnerText += a_Token.value;

			} break;
		}

		return true;
	}

	bool TextParserRtf::_ProcessTextFormatToken(const RtfToken& a_Token, RtfTextFormat& a_TextFormat) const
	{
		if (a_Token.value == "f")
		{
			RtfFont* font = m_Document->GetFontTable()->GetFont(a_Token.parameter);

			a_TextFormat.SetFont(font);

			return true;
		}
		else if (a_Token.value == "fs")
		{
			a_TextFormat.SetFontSize((float)a_Token.parameter / 2.0f);

			return true;
		}
		else if (a_Token.value == "kerning")
		{
			a_TextFormat.SetKerningEnabled((a_Token.parameter == 1) ? true : false);

			return true;
		}
		else if (a_Token.value == "lang")
		{
			const RtfLocale* locale = m_Document->GetWorld()->GetLocaleByIdentifier(a_Token.parameter);

			a_TextFormat.SetLocale(locale);

			return true;
		}
		else if (a_Token.value == "cf")
		{
			RtfColor* color = m_Document->GetColorTable()->GetColor(a_Token.parameter);

			a_TextFormat.SetForegroundColor(color);

			return true;
		}
		else if (a_Token.value == "nowidctlpar")
		{
			a_TextFormat.SetParagraphWidowControl(false);

			return true;
		}
		else
		{
			return false;
		}
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

		_ReadNextToken();
		if (m_TokenCurrent.type != RtfToken::eParseType_Command)
		{
			LOG_ERROR(a_Token) << "Extended control must be followed by another control.";

			return false;
		}

		// check if command is known

		std::map<std::string, CommandHandler>::iterator found = m_GroupCurrent->process_commands->find(m_TokenCurrent.value);

		if (found != m_GroupCurrent->process_commands->end())
		{
			return _ProcessToken(m_TokenCurrent);
		}

		// skip entire group

		while (m_TokenCurrent.type != RtfToken::eParseType_Invalid)
		{
			switch (m_TokenCurrent.type)
			{

			case RtfToken::eParseType_GroupOpen:
				{
					_GroupOpen();

				} break;

			case RtfToken::eParseType_GroupClose:
				{
					_GroupClose();

					if (m_GroupCurrent == group_previous)
					{
						return true;
					}

				} break;
			}

			_ReadNextToken();
		}

		return true;
	}

	bool TextParserRtf::_CommandCharacterSet(const RtfToken& a_Token)
	{
		m_ElementCurrent->GetTextFormat().SetCharacterSet(_TokenToCharset(a_Token));

		return (m_ElementCurrent->GetTextFormat().GetCharacterSet() != eRtfCharacterSet_Invalid);
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

		_ReadNextToken();
		while (m_TokenCurrent.type != RtfToken::eParseType_Invalid)
		{
			switch (m_TokenCurrent.type)
			{

			case RtfToken::eParseType_GroupOpen:
				{
					_GroupOpen();

				} break;

			case RtfToken::eParseType_GroupClose:
				{
					_GroupClose();

					if (m_GroupCurrent == group_fonttable_parent)
					{
						return true;
					}

				} break;

			case RtfToken::eParseType_Command:
				{
					if (m_TokenCurrent.value == "f")
					{
						if (m_TokenCurrent.parameter < 0)
						{
							LOG_ERROR(m_TokenCurrent) << "Invalid parameter \"" << m_TokenCurrent.parameter << "\" for font control.";

							return false;
						}

						font = font_table->GetFont(m_TokenCurrent.parameter);
					}
					else
					{
						if (font == nullptr)
						{
							LOG_ERROR(m_TokenCurrent) << "Invalid control \\" << m_TokenCurrent.value << " because target font was not set.";

							return false;
						}

						// family

						if (m_TokenCurrent.value == "froman")
						{
							font->SetFamilyType(RtfFont::eFamilyType_Roman);
						}
						else if (m_TokenCurrent.value == "fswiss")
						{
							font->SetFamilyType(RtfFont::eFamilyType_Swiss);
						}
						else if (m_TokenCurrent.value == "fmodern")
						{
							font->SetFamilyType(RtfFont::eFamilyType_Modern);
						}
						else if (m_TokenCurrent.value == "fscript")
						{
							font->SetFamilyType(RtfFont::eFamilyType_Script);
						}
						else if (m_TokenCurrent.value == "fdecor")
						{
							font->SetFamilyType(RtfFont::eFamilyType_Decor);
						}
						else if (m_TokenCurrent.value == "ftech")
						{
							font->SetFamilyType(RtfFont::eFamilyType_Tech);
						}
						else if (m_TokenCurrent.value == "fbidi")
						{
							font->SetFamilyType(RtfFont::eFamilyType_Bidi);
						}

						// pitch

						else if (m_TokenCurrent.value == "fprq")
						{
							if (m_TokenCurrent.parameter < RtfFont::ePitch_Default || m_TokenCurrent.parameter > RtfFont::ePitch_Variable)
							{
								LOG_ERROR(m_TokenCurrent) << "Invalid pitch (" << m_TokenCurrent.parameter << ") specified for font.";

								return false;
							}

							font->SetPitch((RtfFont::Pitch)m_TokenCurrent.parameter);
						}

						// characterset

						else if (m_TokenCurrent.value == "fcharset")
						{
							font->SetCharacterSet(_TokenToCharset(m_TokenCurrent));

							if (font->GetCharacterSet() == eRtfCharacterSet_Invalid)
							{
								LOG_ERROR(m_TokenCurrent) << "Invalid character set (" << m_TokenCurrent.parameter << ") specified for font.";

								return false;
							}
						}

						// unhandled

						else
						{
							LOG_WARNING(m_TokenCurrent) << "Unhandled control \"\\" << m_TokenCurrent.value << "\"";
						}
					}

				} break;

			case RtfToken::eParseType_Value:
				{
					font->SetName(m_TokenCurrent.value);

				} break;

			}

			_ReadNextToken();
		}

		return true;
	}

	bool TextParserRtf::_CommandFont(const RtfToken& a_Token)
	{
		if (a_Token.parameter == -1)
		{
			return false;
		}

		m_ElementCurrent->GetTextFormat().SetFont(m_Document->GetFontTable()->GetFont(a_Token.parameter));

		return true;
	}

	bool TextParserRtf::_CommandFontDefault(const RtfToken& a_Token)
	{
		if (a_Token.parameter == -1)
		{
			return false;
		}

		m_Document->GetFontTable()->SetDefault(a_Token.parameter);

		if (m_ElementCurrent->GetTextFormat().GetFont() == nullptr)
		{
			m_ElementCurrent->GetTextFormat().SetFont(m_Document->GetFontTable()->GetDefault());
		}

		return true;
	}

	bool TextParserRtf::_CommandColorTable(const RtfToken& a_Token)
	{
		_ReadNextToken();

		// first empty value indicator

		if (m_TokenCurrent.type != RtfToken::eParseType_Value)
		{
			return false;
		}

		RtfColorTable* color_table = m_Document->GetColorTable();
		RtfColor* color = color_table->AddColor(*color_table->GetDefaultColor());

		_ReadNextToken();

		while (m_TokenCurrent.type != RtfToken::eParseType_Invalid)
		{
			if (m_TokenCurrent.type == RtfToken::eParseType_Command)
			{
				if (m_TokenCurrent.value == "red")
				{
					color->r = m_TokenCurrent.parameter;
				}
				else if (m_TokenCurrent.value == "green")
				{
					color->g = m_TokenCurrent.parameter;
				}
				else if (m_TokenCurrent.value == "blue")
				{
					color->b = m_TokenCurrent.parameter;
				}
				else
				{
					LOG_ERROR(m_TokenCurrent) << "Unknown color control \\" << m_TokenCurrent.parameter << "\\";
				}
			}
			else if (m_TokenCurrent.type == RtfToken::eParseType_Value)
			{
				color = color_table->AddColor(*color_table->GetDefaultColor());
			}
			else
			{
				m_Tokenizer->RevertToken();

				break;
			}

			_ReadNextToken();
		}

		return true;
	}

	bool TextParserRtf::_CommandStyleSheet(const RtfToken& a_Token)
	{
		Group* group_stylesheet_parent = m_GroupCurrent->parent;
		if (group_stylesheet_parent == nullptr)
		{
			LOG_ERROR(a_Token) << "Stylesheet must be inside a group.";

			return false;
		}

		RtfAssociatedProperties* properties = nullptr;
		RtfStyleSheet* stylesheet = m_Document->GetStyleSheet();
		RtfStyle* style = nullptr;

		while (_ReadNextToken() && m_TokenCurrent.type != RtfToken::eParseType_Invalid)
		{
			switch (m_TokenCurrent.type)
			{

			case RtfToken::eParseType_GroupOpen:
				{
					_GroupOpen();

				} break;

			case RtfToken::eParseType_GroupClose:
				{
					_GroupClose();

					if (m_GroupCurrent == group_stylesheet_parent)
					{
						return true;
					}

				} break;

			case RtfToken::eParseType_Command:
				{
					if (m_TokenCurrent.value == "s")
					{
						if (m_TokenCurrent.parameter < 0)
						{
							LOG_ERROR(m_TokenCurrent) << "Invalid parameter \"" << m_TokenCurrent.parameter << "\" for style control word.";

							return false;
						}

						style = stylesheet->GetStyle(m_TokenCurrent.parameter);
					}
					else
					{
						if (style == nullptr)
						{
							LOG_ERROR(m_TokenCurrent) << "Control word \\" << m_TokenCurrent.value << " is invalid because target style was not set.";

							return false;
						}

						if (m_TokenCurrent.value == "snext")
						{
							if (m_TokenCurrent.parameter < 0)
							{
								LOG_ERROR(m_TokenCurrent) << "Invalid parameter \"" << m_TokenCurrent.parameter << "\" for next paragraph style control word.";

								return false;
							}

							RtfStyle* style_next = stylesheet->GetStyle(m_TokenCurrent.parameter);
							style->SetNextParagraphStyle(style_next);
						}
						else if (m_TokenCurrent.value == "loch")
						{
							properties = style->GetPropertiesForCharacterEncoding(eRtfCharacterEncoding_SingleByteLowAnsi);
						}
						else if (m_TokenCurrent.value == "hich")
						{
							properties = style->GetPropertiesForCharacterEncoding(eRtfCharacterEncoding_SingleByteHighAnsi);
						}
						else if (m_TokenCurrent.value == "dbch")
						{
							properties = style->GetPropertiesForCharacterEncoding(eRtfCharacterEncoding_DoubleByte);
						}
						else if (m_TokenCurrent.value == "af")
						{
							RtfFont* font = m_Document->GetFontTable()->GetFont(m_TokenCurrent.parameter);

							properties->SetFont(font);
						}
						else if (m_TokenCurrent.value == "afs")
						{
							properties->SetFontSize((float)m_TokenCurrent.parameter / 2.0f);
						}
						else if (m_TokenCurrent.value == "alang")
						{
							const RtfLocale* locale = m_Document->GetWorld()->GetLocaleByIdentifier(m_TokenCurrent.parameter);

							properties->SetLocale(locale);
						}
						else if (!_ProcessTextFormatToken(m_TokenCurrent, style->GetTextFormat()))
						{
							LOG_ERROR(m_TokenCurrent) << "Unhandled control word \\" << m_TokenCurrent.value << ".";
						}
					}

				} break;

			case RtfToken::eParseType_Value:
				{
					if (style == nullptr)
					{
						LOG_ERROR(m_TokenCurrent) << "Value " << m_TokenCurrent.value << " is invalid because target style was not set.";

						return false;
					}

					style->SetName(m_TokenCurrent.value);

				} break;

			}
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