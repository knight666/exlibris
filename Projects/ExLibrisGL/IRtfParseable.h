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

#include "RtfParserGroup.h"
#include "RtfParserState.h"
#include "RtfToken.h"

namespace ExLibris
{

	class IRtfParseable
	{

	public:

		enum Result
		{
			eResult_Invalid,
			eResult_Propagate,
			eResult_Handled,
			eResult_Finished,
		};

	public:

		IRtfParseable(IRtfParseable* a_ParserParent = nullptr)
			: m_ParserParent(a_ParserParent)
		{
		}
		virtual ~IRtfParseable()
		{
		}

		Result Parse(RtfParserState& a_State, const RtfToken& a_Token)
		{
			Result result = eResult_Propagate;

			switch (a_Token.type)
			{

			case RtfToken::eParseType_GroupOpen:
				{
					a_State.PushGroup();

					result = eResult_Handled;

				} break;

			case RtfToken::eParseType_GroupClose:
				{
					a_State.PopGroup();

					result = eResult_Handled;

				} break;

			case RtfToken::eParseType_Command:
				{
					result = _ParseCommand(a_State, a_Token);

				} break;

			case RtfToken::eParseType_CommandExtended:
				{
					result = _ParseCommandExtended(a_State, a_Token);

				} break;

			case RtfToken::eParseType_Value:
				{
					result = _ParseValue(a_State, a_Token);

				} break;

			case RtfToken::eParseType_Text:
				{
					result = _ParseText(a_State, a_Token);

				} break;

			}

			return result;
		}

		IRtfParseable* GetParserParent() const
		{
			return m_ParserParent;
		}

	protected:

		virtual Result _ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
		{
			return eResult_Propagate;
		}

		virtual Result _ParseCommandExtended(RtfParserState& a_State, const RtfToken& a_Token)
		{
			return eResult_Propagate;
		}

		virtual Result _ParseValue(RtfParserState& a_State, const RtfToken& a_Token)
		{
			return eResult_Propagate;
		}

		virtual Result _ParseText(RtfParserState& a_State, const RtfToken& a_Token)
		{
			return eResult_Propagate;
		}

		IRtfParseable* m_ParserParent;

	}; // class IRtfParseable

}; // namespace ExLibris