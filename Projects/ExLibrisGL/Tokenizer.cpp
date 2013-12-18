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
#define TYPE_CLASS(_name, _condition) \
	class TYPE_CLASS_NAME(_name) { \
		public: \
			static inline bool IsKnown(int c) { \
				return _condition; \
			} \
	}

	TYPE_CLASS(Digit, (c >= '0' && c <= '9'));
	TYPE_CLASS(Octal, (c >= '0' && c <= '7'));
	TYPE_CLASS(Hexadecimal, (
		(c >= '0' && c <= '9') || 
		(c >= 'a' && c <= 'f') ||
		(c >= 'A' && c <= 'F')
	));
	TYPE_CLASS(Alphabetical, (
		(c >= 'A' && c <= 'Z') ||
		(c >= 'a' && c <= 'z')
	));
	TYPE_CLASS(Symbol, (
		c == '!' || c == '\"' || c == '#' || c == '$' || c == '%' ||
		c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' ||
		c == '+' || c == ',' || c == '-' || c == '.' || c == '/' ||
		c == ':' || c == ';' || c == '<' || c == '=' || c == '>' ||
		c == '?' || c == '@' || c == '[' || c == '\\' || c == ']' ||
		c == '^' || c == '_' || c == '`' || c == '{' || c == '|' ||
		c == '}' || c == '~'
	));
	TYPE_CLASS(Whitespace, (c == ' ' || c == '\t'));
	TYPE_CLASS(NewLine, (c == '\r' || c == '\n'));

	Tokenizer::Tokenizer(std::basic_istream<char>* a_Stream)
		: m_Stream(nullptr)
		, m_StreamEnd(false)
		, m_TabWidth(4)
		, m_CharactersConsumedCount(0)
		, m_FoundFloatingDot(false)
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

	void Tokenizer::SetTabWidth(size_t a_Width)
	{
		m_TabWidth = std::max<size_t>(1, a_Width);
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
		m_CharactersRead.clear();
		m_CharactersConsumedCount = 0;

		m_FoundFloatingDot = false;
		m_StringDelimiter = -1;

		m_TokenCurrent.text.clear();
		m_TokenCurrent.column = m_Column;
		m_TokenCurrent.line = m_Line;

		if (!_IsNextAvailable() || !_NextCharacter())
		{
			m_TokenCurrent.type = Token::eType_End;

			return false;
		}

		if (m_CharacterCurrent == '.')
		{
			m_TokenCurrent.type = Token::eType_Number;

			return _RecursiveReadToken();
		}
		else if (m_CharacterCurrent == '0')
		{
			m_TokenCurrent.type = Token::eType_Octal;

			return _RecursiveReadToken();
		}
		else if (m_CharacterCurrent == '\r')
		{
			m_TokenCurrent.type = Token::eType_Unprintable;

			return _RecursiveReadToken();
		}
		else if (m_CharacterCurrent == '\n')
		{
			m_TokenCurrent.type = Token::eType_NewLine;

			return _RecursiveReadToken();
		}
		else if (m_CharacterCurrent == '\"' || m_CharacterCurrent == '\'')
		{
			m_TokenCurrent.type = Token::eType_String;

			return _RecursiveReadToken();
		}
		else if (m_CharacterCurrent == ' ' || m_CharacterCurrent == '\t')
		{
			m_TokenCurrent.type = Token::eType_Whitespace;

			return _RecursiveReadToken();
		}
		else if (_IsCharacterOfType<CharacterTypeDigit>(m_CharacterCurrent))
		{
			m_TokenCurrent.type = Token::eType_Integer;

			return _RecursiveReadToken();
		}
		else if (_IsCharacterOfType<CharacterTypeAlphabetical>(m_CharacterCurrent))
		{
			m_TokenCurrent.type = Token::eType_Text;

			return _RecursiveReadToken();
		}
		else if (_TryConsumeOne<CharacterTypeSymbol>())
		{
			m_TokenCurrent.type = Token::eType_Symbol;

			return true;
		}

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

	bool Tokenizer::_RecursiveReadToken()
	{
		bool handled = false;

		switch (m_TokenCurrent.type)
		{

		case Token::eType_Text:
			{
				if (!_IsNextAvailable())
				{
					_TryConsumeOne<CharacterTypeAlphabetical>();

					return true;
				}
				else if (!_TryConsumeOne<CharacterTypeAlphabetical>())
				{
					if (_IsNextAvailable())
					{
						_Revert(1);
					}

					return true;
				}

			} break;

		case Token::eType_String:
			{
				if (_TryConsume(m_StringDelimiter))
				{
					return true;
				}
				else if (m_CharactersConsumedCount == 0 && (_TryConsume('\'') || _TryConsume('\"')))
				{
					m_StringDelimiter = m_CharacterCurrent;
				}
				else if (m_CharacterCurrent == '\r')
				{
					_NextCharacter();

					if (m_CharacterCurrent == '\n' || !_IsNextAvailable())
					{
						// undo consumed

						_Revert(m_CharactersRead.size());

						// quote character was just a symbol

						m_TokenCurrent.type = Token::eType_Symbol;
						m_TokenCurrent.text.clear();
						_NextCharacter();
						_AddCurrentToToken();

						return true;
					}
					else
					{
						_Revert(2);

						_NextCharacter();
						_AddCurrentToToken();
					}
				}
				else if (m_CharacterCurrent == '\n' || !_IsNextAvailable())
				{
					// undo consumed

					_Revert(m_CharactersRead.size());

					// quote character was just a symbol

					m_TokenCurrent.type = Token::eType_Symbol;
					m_TokenCurrent.text.clear();
					_NextCharacter();
					_AddCurrentToToken();

					return true;
				}
				else
				{
					_AddCurrentToToken();
				}

			} break;

		case Token::eType_Integer:
			{
				if (m_CharacterCurrent == '.')
				{
					m_TokenCurrent.type = Token::eType_Number;

					return _RecursiveReadToken();
				}
				else if (!_TryConsumeOne<CharacterTypeDigit>())
				{
					handled = true;
				}

			} break;

		case Token::eType_Octal:
			{
				if (m_CharacterCurrent == '.')
				{
					m_TokenCurrent.type = Token::eType_Number;

					return _RecursiveReadToken();
				}
				else if (m_CharactersConsumedCount == 1 && _TryConsume('x'))
				{
					m_TokenCurrent.type = Token::eType_Hexadecimal;
				}
				else if (!_TryConsumeOne<CharacterTypeOctal>())
				{
					if (m_CharactersConsumedCount == 1)
					{
						// just a zero

						m_TokenCurrent.type = Token::eType_Integer;
					}

					handled = true;
				}

			} break;

		case Token::eType_Hexadecimal:
			{
				if (!_TryConsumeOne<CharacterTypeHexadecimal>())
				{
					if (m_CharactersConsumedCount == 2)
					{
						// was a zero with text

						m_TokenCurrent.type = Token::eType_Integer;

						_Revert(3);

						m_TokenCurrent.text.clear();

						_NextCharacter();
						_AddCurrentToToken();

						return true;
					}
					else
					{
						handled = true;
					}
				}

			} break;

		case Token::eType_Number:
			{
				if (_TryConsume('f') || _TryConsume('F'))
				{
					// no more digits after specifier

					return true;
				}
				else if (m_CharacterCurrent == '.')
				{
					if (m_FoundFloatingDot)
					{
						if (m_CharactersConsumedCount == 0)
						{
							// just a dot

							m_TokenCurrent.type = Token::eType_Symbol;
						}

						handled = true;
					}
					else
					{
						_NextCharacter();

						if (m_CharacterCurrent == 'f' || m_CharacterCurrent == 'F' || _IsCharacterOfType<CharacterTypeDigit>(m_CharacterCurrent))
						{
							m_TokenCurrent.text.push_back('.');

							_AddCurrentToToken();

							m_FoundFloatingDot = true;
						}
						else if (!_IsNextAvailable())
						{
							if (m_CharactersConsumedCount == 0)
							{
								// dot only

								m_TokenCurrent.type = Token::eType_Symbol;
								m_TokenCurrent.text.push_back('.');
							}
							else
							{
								// trailing dot

								_Revert(1);

								m_TokenCurrent.type = Token::eType_Integer;
							}

							// make sure the next token starts at the correct position

							m_Column--;

							return true;
						}
						else
						{
							if (m_CharactersConsumedCount == 0)
							{
								// leading dot

								_Revert(1);

								m_TokenCurrent.type = Token::eType_Symbol;
								m_TokenCurrent.text.push_back('.');
							}
							else
							{
								// trailing dot

								_Revert(2);

								m_TokenCurrent.type = Token::eType_Integer;
							}

							return true;
						}
					}
				}
				else if (!_TryConsumeOne<CharacterTypeDigit>())
				{
					handled = true;
				}

			} break;

		case Token::eType_Whitespace:
			{
				if (_TryConsume('\t'))
				{
					// tab position is determined from index, not column

					m_Column = m_TokenCurrent.column + (m_TabWidth - ((m_TokenCurrent.column - 1) % m_TabWidth));

					return true;
				}
				else if (_TryConsume(' '))
				{
					return true;
				}
				else
				{
					handled = true;
				}

			} break;

		case Token::eType_Unprintable:
			{
				if (_TryConsume('\r'))
				{
					_NextCharacter();

					if (m_CharacterCurrent == '\n')
					{
						// carriage return and newline combine to a newline

						m_TokenCurrent.type = Token::eType_NewLine;

						_Revert(1);
					}
					else
					{
						// just a carriage return

						if (_IsNextAvailable())
						{
							_Revert(1);
						}
						else
						{
							m_Column--;
						}

						return true;
					}
				}

			} break;

		case Token::eType_NewLine:
			{
				if (_TryConsume('\n'))
				{
					m_Column = 1;
					m_Line++;

					return true;
				}

			} break;

		default:
			{
				return false;

			} break;

		}

		bool next_available = _IsNextAvailable();

		if (handled)
		{
			if (next_available)
			{
				_Revert(1);
			}

			return true;
		}
		else
		{
			if (next_available)
			{
				_ReadNextCharacter();
			}

			return _RecursiveReadToken();
		}
	}

	bool Tokenizer::_IsNextCharacterAvailable() const
	{
		return ((m_Stream != nullptr && !m_Stream->eof()) || (m_CharacterQueue.size() > 0) && m_CharacterCurrent != -1);
	}

	bool Tokenizer::_IsNextAvailable() const
	{
		return ((m_Stream != nullptr) && !m_Stream->eof()) || (m_CharacterQueue.size() > 0);
	}

	bool Tokenizer::_NextCharacter()
	{
		bool add = false;

		if (m_CharacterQueue.size() > 0)
		{
			m_CharacterCurrent = m_CharacterQueue.front();
			m_CharacterQueue.pop_front();

			add = true;
		}
		else if (m_Stream != nullptr && !m_Stream->eof())
		{
			m_CharacterCurrent = m_Stream->get();

			add = !m_Stream->eof();
		}

		if (add)
		{
			m_CharactersRead.push_back(m_CharacterCurrent);
		}

		m_Column++;

		return add;
	}

	void Tokenizer::_Revert(int a_Count)
	{
		int added = 0;

		while (added < a_Count && m_CharactersRead.size() > 0)
		{
			m_CharacterQueue.push_front(m_CharactersRead.back());
			m_CharactersRead.pop_back();

			++added;
		}

		m_Column -= added;
	}

	void Tokenizer::_AddCurrentToToken()
	{
		m_TokenCurrent.text.push_back((char)m_CharacterCurrent);
		m_CharactersConsumedCount++;
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

	bool Tokenizer::_ReadNextCharacter()
	{
		bool next_available = false;

		if (m_CharacterQueue.size() > 0)
		{
			next_available = true;

			m_CharacterCurrent = m_CharacterQueue.front();
			m_CharacterQueue.pop_front();
		}
		else
		{
			next_available = (m_Stream != nullptr) && !m_Stream->eof();
			if (next_available)
			{
				m_CharacterCurrent = m_Stream->get();

				next_available = !m_Stream->eof();
			}
		}

		if (next_available)
		{
			m_Column++;

			m_CharactersRead.push_back(m_CharacterCurrent);
		}

		return next_available;
	}

}; // namespace ExLibris