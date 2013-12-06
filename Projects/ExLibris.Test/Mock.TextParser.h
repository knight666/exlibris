#pragma once

#include <ITextParser.h>

namespace ExLibris
{

	class MockTextParser
		: public ITextParser
	{
	
	public:

		void AddTokenString(const std::string& a_Text)
		{
			for (std::string::const_iterator char_it = a_Text.begin(); char_it != a_Text.end(); ++char_it)
			{
				TextParserToken token;
				token.codepoint = *char_it;

				switch (token.codepoint)
				{

				case ' ':
				case '\t':
					{
						token.type = TextParserToken::eCodepointType_Whitespace;

					} break;

				case '\n':
					{
						token.type = TextParserToken::eCodepointType_Newline;

					} break;

				default:
					{
						if (token.codepoint < 32)
						{
							token.type = TextParserToken::eCodepointType_Unprintable;
						}
						else
						{
							token.type = TextParserToken::eCodepointType_Character;
						}

					} break;

				}

				text.push_back(*char_it);
				tokens.push(token);
			}
		}

		void AddTokenStyle(const TextParserToken& a_Token)
		{
			tokens.push(a_Token);
		}

		void SetInput(const std::string& a_Text)
		{
			AddTokenString(a_Text);
		}

		bool ReadToken()
		{
			if (tokens.empty())
			{
				return false;
			}
			else
			{
				m_Token = tokens.front();
				tokens.pop();

				return true;
			}
		}

	public:

		std::string text;
		std::queue<TextParserToken> tokens;
	
	}; // class MockTextParser

}; // namespace ExLibris
