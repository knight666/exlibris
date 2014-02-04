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

#include "Exception.h"
#include "IRtfParseable.h"
#include "RtfColorTable.h"
#include "RtfDomElement.h"
#include "RtfFontTable.h"
#include "RtfStyleSheet.h"
#include "RtfTextFormat.h"
#include "RtfWorld.h"

namespace ExLibris
{
	class RtfDomElement;

	namespace Rtf
	{
		class Tokenizer;
	}
};

namespace ExLibris
{

	class RtfDomDocument
		: public IRtfParseable
	{

	public:

		RtfDomDocument(RtfWorld* a_World);
		~RtfDomDocument();

		RtfTextFormat& GetTextFormat();

		RtfDomElement* GetRootElement() const;

		RtfWorld* GetWorld() const;
		RtfFontTable* GetFontTable() const;
		RtfColorTable* GetColorTable() const;
		RtfStyleSheet* GetStyleSheet() const;

		bool GetWidowControl() const;
		void SetWidowControl(bool a_Value);

		bool ParseFromSource(std::basic_istream<char>* a_Stream);

	private:

		IRtfParseable::Result _ParseCommand(RtfParserState& a_State, const RtfToken& a_Token);
		IRtfParseable::Result _ParseText(RtfParserState& a_State, const RtfToken& a_Token);

	private:

		RtfTextFormat* m_TextFormat;

		RtfDomElement* m_RootElement;

		RtfWorld* m_World;
		RtfFontTable* m_FontTable;
		RtfColorTable* m_ColorTable;
		RtfStyleSheet* m_StyleSheet;

		bool m_WidowControl;

		Rtf::Tokenizer* m_Tokenizer;

		struct ParseState;
		ParseState* m_State;

	}; // class RtfDomDocument

}; // namespace ExLibris