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

#include "IRtfParseable.h"
#include "RtfTextFormat.h"

namespace ExLibris
{
	class RtfDomDocument;
}

namespace ExLibris
{

	class RtfDomElement
		: public IRtfParseable
	{

		friend class RtfDomElement;

	public:

		RtfDomElement(RtfDomDocument& a_Document);
		~RtfDomElement();

		std::string InnerText;

		RtfTextFormat& GetTextFormat() const;

		RtfDomElement* GetParent() const;

		RtfDomElement* GetSiblingPrevious() const;
		RtfDomElement* GetSiblingNext() const;

		size_t GetChildrenCount() const;
		RtfDomElement* GetChild(size_t a_Index) const;
		RtfDomElement* AddChild();

	private:

		IRtfParseable::Result _ParseCommand(RtfParserState& a_State, const RtfToken& a_Token);
		IRtfParseable::Result _ParseText(RtfParserState& a_State, const RtfToken& a_Token);

	private:

		RtfDomDocument& m_Document;
		RtfTextFormat* m_TextFormat;
		RtfDomElement* m_Parent;
		RtfDomElement* m_SiblingPrevious;
		RtfDomElement* m_SiblingNext;
		std::vector<RtfDomElement*> m_Children;
		RtfDomElement* m_ChildPrevious;

		struct ParseState;
		ParseState* m_State;

	}; // class RtfDomElement

}; // namespace ExLibris