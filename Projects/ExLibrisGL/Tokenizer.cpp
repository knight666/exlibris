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

#include "Tokenizer.h"

namespace ExLibris
{


#define TYPE_CLASS_NAME(_name) CharacterType##_name
#define TYPE_CLASS(_name, ...) \
	class TYPE_CLASS_NAME(_name) { \
		public: \
			static inline bool IsKnown(int a_Character) { \
				static const int identifiers[] = { __VA_ARGS__ }; \
				static const int identifiers_size = sizeof(identifiers) / sizeof(int); \
				for (int i = 0; i < identifiers_size; ++i) { \
					if (a_Character == identifiers[i]) { return true; } \
				} \
				return false; \
			} \
	} \

	TYPE_CLASS(Digit,
		'0', '1', '2', '3', '4',
		'5', '6', '7', '8', '9'
	);
	TYPE_CLASS(Octal,
		'0', '1', '2', '3',
		'4', '5', '6', '7'
	);
	TYPE_CLASS(Hexadecimal,
		'0', '1', '2', '3', '4',
		'5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f',
		'A', 'B', 'C', 'D', 'E', 'F',
	);
	TYPE_CLASS(Symbol, 
		'!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', 
		',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', 
		'[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'
	);
	TYPE_CLASS(Whitespace,
		' ', '\t'
	);
	TYPE_CLASS(NewLine,
		'\r', '\n'
	);

	Tokenizer::Tokenizer(std::basic_istream<char>* a_Stream)
		: m_Stream(nullptr)
		, m_CharacterCurrent(-1)
		, m_Column(0)
		, m_Line(0)
	{
		SetInput(a_Stream);
	}
	
	Tokenizer::~Tokenizer()
	{
	}

	void Tokenizer::SetInput(std::basic_istream<char>* a_Stream)
	{
		m_Stream = a_Stream;

		m_Column = 1;
		m_Line = 1;
	}

	const Token& Tokenizer::GetCurrentToken() const
	{
		return m_TokenCurrent;
	}

	bool Tokenizer::IsNextTokenAvailable() const
	{
		return ((m_Stream != nullptr && !m_Stream->eof()) || (m_CharacterQueue.size() > 0));
	}

	bool Tokenizer::ReadToken()
	{
		m_TokenCurrent.text.clear();

		m_TokenCurrent.column = m_Column;
		m_TokenCurrent.line = m_Line;

		if (!_IsNextCharacterAvailable())
		{
			m_TokenCurrent.type = Token::eType_End;

			return false;
		}

		_NextCharacter();

		if (m_CharacterCurrent == -1)
		{
			m_TokenCurrent.type = Token::eType_End;

			return false;
		}

		m_CharactersUndoConsumed.clear();

		if (
			!_ConsumeNumber() &&
			!_ConsumeString() &&
			!_ConsumeSymbol() &&
			!_ConsumeWhitespace() &&
			!_ConsumeNewLine()
		)
		{
			_ConsumeText();
		}

		return true;
	}

	bool Tokenizer::_IsNextCharacterAvailable() const
	{
		return ((m_Stream != nullptr && !m_Stream->eof()) || (m_CharacterQueue.size() > 0) && m_CharacterCurrent != -1);
	}

	void Tokenizer::_NextCharacter()
	{
		if (m_CharacterQueue.size() > 0)
		{
			m_CharacterCurrent = m_CharacterQueue.front();
			m_CharacterQueue.pop_front();
		}
		else
		{
			m_CharacterCurrent = m_Stream->get();
		}

		m_Column++;
	}

	void Tokenizer::_AddCurrentToToken()
	{
		m_TokenCurrent.text.push_back((char)m_CharacterCurrent);
	}

	void Tokenizer::_QueueCurrentCharacter()
	{
		m_CharacterQueue.push_front(m_CharacterCurrent);
		m_Column--;
	}

	void Tokenizer::_UndoConsumed()
	{
		m_CharacterQueue.insert(m_CharacterQueue.end(), m_CharactersUndoConsumed.begin(), m_CharactersUndoConsumed.end());
		m_Column -= (int)m_CharactersUndoConsumed.size();

		m_TokenCurrent.text.clear();
		m_CharacterCurrent = m_CharacterRestore;
	}

	Token::Type Tokenizer::_GetTypeForCharacter(int a_Character)
	{
		if (a_Character == -1)
		{
			return Token::eType_End;
		}

		Token::Type found_type = Token::eType_Text;

		if (_IsCharacterOfType<CharacterTypeDigit>(a_Character))
		{
			found_type = Token::eType_Integer;
		}
		else if (_IsCharacterOfType<CharacterTypeSymbol>(a_Character))
		{
			found_type = Token::eType_Symbol;
		}
		else if (_IsCharacterOfType<CharacterTypeWhitespace>(a_Character))
		{
			found_type = Token::eType_Whitespace;
		}
		else if (_IsCharacterOfType<CharacterTypeNewLine>(a_Character))
		{
			found_type = Token::eType_NewLine;
		}

		return found_type;
	}

	bool Tokenizer::_TryConsume(int a_Character)
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

	bool Tokenizer::_ConsumeNumber()
	{
		// is it a floating point number?

		if (_TryConsume('.'))
		{
			m_CharacterRestore = m_CharacterCurrent;

			_NextCharacter();

			if (!_ConsumeNumberFloat())
			{
				// it was just a dot

				m_TokenCurrent.type = Token::eType_Symbol;
				m_TokenCurrent.text.push_back('.');
			}
			
			return true;
		}

		if (!_IsCharacterOfType<CharacterTypeDigit>(m_CharacterCurrent))
		{
			return false;
		}

		bool is_number = false;

		// is it an octal or hexadecimal number?

		if (_TryConsume('0'))
		{
			m_CharacterRestore = '0';

			_NextCharacter();

			if (_IsNextCharacterAvailable())
			{
				if (_TryConsume('x'))
				{
					is_number = _ConsumeNumberHexadecimal();
				}
				else
				{
					_QueueCurrentCharacter();

					is_number = _ConsumeNumberOctal();
				}
			}
			else
			{
				// sometimes a zero is just a zero

				m_TokenCurrent.type = Token::eType_Integer;

				_QueueCurrentCharacter();

				is_number = true;
			}
		}

		if (!is_number)
		{
			is_number = _ConsumeNumberInteger();
		}

		return is_number;
	}

	bool Tokenizer::_ConsumeNumberInteger()
	{
		if (!_IsCharacterOfType<CharacterTypeDigit>(m_CharacterCurrent))
		{
			return false;
		}

		m_CharacterRestore = m_CharacterCurrent;

		m_TokenCurrent.type = Token::eType_Integer;

		_AddCurrentToToken();

		while (_IsNextCharacterAvailable())
		{
			_NextCharacter();

			m_CharactersUndoConsumed.push_back(m_CharacterCurrent);

			if (m_CharacterCurrent == '.')
			{
				_NextCharacter();

				if (_IsNextCharacterAvailable() && (_IsCharacterOfType<CharacterTypeDigit>(m_CharacterCurrent) || m_CharacterCurrent == 'f' || m_CharacterCurrent == 'F'))
				{
					m_TokenCurrent.text.push_back('.');

					m_CharacterRestore = m_CharacterCurrent;

					_ConsumeNumberFloat();
				}
				else
				{
					_QueueCurrentCharacter();

					m_CharacterQueue.push_front('.');
					m_Column--;

					m_CharacterCurrent = '.';
				}

				break;
			}

			if (!_IsCharacterOfType<CharacterTypeDigit>(m_CharacterCurrent))
			{
				_QueueCurrentCharacter();

				break;
			}

			_AddCurrentToToken();
		}

		return true;
	}

	bool Tokenizer::_ConsumeNumberOctal()
	{
		int valid = 0;

		while (_IsNextCharacterAvailable())
		{
			_NextCharacter();

			m_CharactersUndoConsumed.push_back(m_CharacterCurrent);

			if (m_CharacterCurrent == '.')
			{
				_NextCharacter();

				if (_IsNextCharacterAvailable() && (_IsCharacterOfType<CharacterTypeDigit>(m_CharacterCurrent) || m_CharacterCurrent == 'f' || m_CharacterCurrent == 'F'))
				{
					m_TokenCurrent.text.push_back('.');

					m_CharacterRestore = m_CharacterCurrent;

					return _ConsumeNumberFloat();
				}
				else
				{
					_QueueCurrentCharacter();

					m_CharacterQueue.push_front('.');
					m_Column--;

					m_CharacterCurrent = '.';

					break;
				}
			}

			if (!_IsCharacterOfType<CharacterTypeOctal>(m_CharacterCurrent))
			{
				break;
			}

			_AddCurrentToToken();
			valid++;
		}

		if (valid == 0)
		{
			_UndoConsumed();

			return false;
		}
		else
		{
			m_TokenCurrent.type = Token::eType_Octal;

			_QueueCurrentCharacter();

			return true;
		}
	}

	bool Tokenizer::_ConsumeNumberHexadecimal()
	{
		m_CharactersUndoConsumed.push_back(m_CharacterCurrent);

		int valid = 0;

		while (_IsNextCharacterAvailable())
		{
			_NextCharacter();

			m_CharactersUndoConsumed.push_back(m_CharacterCurrent);

			if (m_CharacterCurrent == '.' || !_IsCharacterOfType<CharacterTypeHexadecimal>(m_CharacterCurrent))
			{
				break;
			}

			_AddCurrentToToken();
			valid++;
		}

		if (valid == 0)
		{
			_UndoConsumed();

			return false;
		}
		else
		{
			m_TokenCurrent.type = Token::eType_Hexadecimal;

			_QueueCurrentCharacter();

			return true;
		}
	}

	bool Tokenizer::_ConsumeNumberFloat()
	{
		if (!_IsCharacterOfType<CharacterTypeDigit>(m_CharacterCurrent))
		{
			if (_IsNextCharacterAvailable() && (_TryConsume('f') || _TryConsume('F')))
			{
				m_TokenCurrent.type = Token::eType_Number;

				return true;
			}
			else
			{
				_QueueCurrentCharacter();
				_UndoConsumed();

				return false;
			}
		}

		_AddCurrentToToken();

		int found = 0;

		while (_IsNextCharacterAvailable())
		{
			_NextCharacter();

			m_CharactersUndoConsumed.push_back(m_CharacterCurrent);

			if (m_CharacterCurrent == '.' || !_IsCharacterOfType<CharacterTypeDigit>(m_CharacterCurrent))
			{
				if (_TryConsume('f') || _TryConsume('F'))
				{
					found++;
				}
				else
				{
					_QueueCurrentCharacter();
				}

				break;
			}

			_AddCurrentToToken();
			found++;
		}

		m_TokenCurrent.type = Token::eType_Number;

		return true;
	}

	bool Tokenizer::_ConsumeString()
	{
		if (_TryConsume('\"') || _TryConsume('\''))
		{
			m_TokenCurrent.type = Token::eType_String;

			int delimiter = m_CharacterCurrent;

			m_CharacterRestore = delimiter;
			bool undo = false;

			while (_IsNextCharacterAvailable())
			{
				_NextCharacter();

				m_CharactersUndoConsumed.push_back(m_CharacterCurrent);

				if (m_CharacterCurrent == delimiter)
				{
					_AddCurrentToToken();

					break;
				}
				else if (m_CharacterCurrent == -1)
				{
					undo = true;

					break;
				}
				else
				{
					Token::Type type_found = _GetTypeForCharacter(m_CharacterCurrent);
					if (type_found == Token::eType_NewLine)
					{
						undo = true;

						break;
					}
				}

				_AddCurrentToToken();
			}

			if (undo)
			{
				_UndoConsumed();
			}

			return !undo;
		}
		else
		{
			return false;
		}
	}

	bool Tokenizer::_ConsumeSymbol()
	{
		if (_TryConsumeOne<CharacterTypeSymbol>())
		{
			m_TokenCurrent.type = Token::eType_Symbol;

			return true;
		}
		else
		{
			return false;
		}
	}

	bool Tokenizer::_ConsumeNewLine()
	{
		if (_TryConsume('\r'))
		{
			_NextCharacter();

			// carriage return followed by line feed?

			if (_TryConsume('\n'))
			{
				m_TokenCurrent.type = Token::eType_NewLine;

				m_Column = 1;
				m_Line++;
			}
			else
			{
				// just a carriage return

				m_TokenCurrent.type = Token::eType_Unprintable;

				_QueueCurrentCharacter();
			}

			return true;
		}
		else if (_TryConsume('\n'))
		{
			m_TokenCurrent.type = Token::eType_NewLine;

			m_Column = 1;
			m_Line++;

			return true;
		}
		else
		{
			return false;
		}
	}

	bool Tokenizer::_ConsumeWhitespace()
	{
		if (_TryConsumeOne<CharacterTypeWhitespace>())
		{
			m_TokenCurrent.type = Token::eType_Whitespace;

			return true;
		}
		else
		{
			return false;
		}
	}

	void Tokenizer::_ConsumeText()
	{
		m_TokenCurrent.type = Token::eType_Text;

		_AddCurrentToToken();

		while (_IsNextCharacterAvailable())
		{
			_NextCharacter();

			Token::Type type_found = _GetTypeForCharacter(m_CharacterCurrent);
			if (type_found != Token::eType_Text)
			{
				_QueueCurrentCharacter();

				break;
			}

			_AddCurrentToToken();
		}
	}

}; // namespace ExLibris"