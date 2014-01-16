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

		virtual ~IRtfParseable()
		{
		}

		Result Parse(RtfParserState& a_State, const RtfToken& a_Token)
		{
			Result result = eResult_Handled;

			switch (a_Token.type)
			{

			case RtfToken::eParseType_GroupOpen:
				{
					result = _ParseGroupOpen(a_State, a_Token);

				} break;

			case RtfToken::eParseType_GroupClose:
				{
					result = _ParseGroupClose(a_State, a_Token);

				} break;

			case RtfToken::eParseType_Command:
				{
					result = _ParseCommand(a_State, a_Token);

				} break;

			case RtfToken::eParseType_Value:
				{
					result = _ParseValue(a_State, a_Token);

				} break;

			}

			if (result == eResult_Finished)
			{
				_PopTarget(a_State);

				return eResult_Handled;
			}
			else
			{
				return result;
			}
		}

	protected:

		virtual Result _ParseGroupOpen(RtfParserState& a_State, const RtfToken& a_Token)
		{
			RtfParserGroup* group_create = new RtfParserGroup;
			group_create->index = ++a_State.group_index;
			group_create->parent = a_State.group_current;

			a_State.group_current = group_create;

			return eResult_Handled;
		}

		virtual Result _ParseGroupClose(RtfParserState& a_State, const RtfToken& a_Token)
		{
			a_State.group_index--;

			if (a_State.group_current != nullptr)
			{
				a_State.group_current = a_State.group_current->parent;
			}

			return eResult_Handled;
		}

		virtual Result _ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
		{
			return eResult_Propagate;
		}

		virtual Result _ParseValue(RtfParserState& a_State, const RtfToken& a_Token)
		{
			return eResult_Handled;
		}

		void _PushTarget(RtfParserState& a_State, IRtfParseable* a_Target)
		{
			if (a_Target == nullptr)
			{
				return;
			}

			a_State.target = a_Target;
			a_State.parseables.push(this);
		}

		void _PopTarget(RtfParserState& a_State)
		{
			if (a_State.parseables.size() == 0)
			{
				return;
			}

			a_State.target = a_State.parseables.top();
			a_State.parseables.pop();
		}

	}; // class IRtfParseable

}; // namespace ExLibris