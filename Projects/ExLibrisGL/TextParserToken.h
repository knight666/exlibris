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

#include "Style.h"
#include "Weight.h"

namespace ExLibris
{

	struct TextParserToken
	{

		enum CodepointType
		{
			eCodepointType_Character,
			eCodepointType_Space,
			eCodepointType_Tab,
			eCodepointType_Newline
		};

		enum Changed
		{
			eChanged_None            = 0,
			eChanged_FamilyName      = (1 << 0),
			eChanged_Size            = (1 << 1),
			eChanged_Weight          = (1 << 2),
			eChanged_Style           = (1 << 3),
			eChanged_Color           = (1 << 4),
			eChanged_BackgroundColor = (1 << 5),
		};

	public:

		TextParserToken()
			: codepoint(0)
			, type(eCodepointType_Character)
			, changes(eChanged_None)
			, size(0.0f)
			, weight(eWeight_Normal)
			, style(eStyle_None)
			, color(0.0f, 0.0f, 0.0f, 1.0f)
			, background_color(1.0f, 1.0f, 1.0f, 1.0f)
		{
		}

	public:

		int codepoint;
		CodepointType type;

		unsigned int changes;
		std::string family_name;
		float size;
		Weight weight;
		Style style;
		glm::vec4 color;
		glm::vec4 background_color;

	};

}; // namespace ExLibris