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

#include "RtfDomDocument.h"

#include "RtfDomElement.h"

namespace ExLibris
{

	struct RtfDomDocument::ParseState
	{
		ParseState()
			: element(nullptr)
			, character_set_found(false)
		{
		}

		RtfDomElement* element;
		bool character_set_found;
	};

	RtfDomDocument::RtfDomDocument(RtfWorld* a_World)
		: m_World(a_World)
		, m_RootElement(nullptr)
		, m_TextFormat(nullptr)
		, m_WidowControl(true)
		, m_State(new ParseState)
	{
		m_FontTable = new RtfFontTable(*this);
		m_ColorTable = new RtfColorTable(*this);
		m_StyleSheet = new RtfStyleSheet(*this);

		m_TextFormat = new RtfTextFormat(*this);
		m_TextFormat->SetCharacterSet(eRtfCharacterSet_Invalid);
		m_TextFormat->SetCharacterEncoding(eRtfCharacterEncoding_SingleByteLowAnsi);
		m_TextFormat->SetFont(m_FontTable->GetFont(0));
		m_TextFormat->SetLocale(nullptr);
		m_TextFormat->SetBackgroundColor(m_ColorTable->GetColor(0));
		m_TextFormat->SetForegroundColor(m_ColorTable->GetColor(0));
		m_TextFormat->SetParagraphWidowControl(true);
		m_TextFormat->SetKerningEnabled(true);
		m_TextFormat->SetMinimumKerningSize(1);
	}

	RtfDomDocument::~RtfDomDocument()
	{
		delete m_FontTable;
		delete m_ColorTable;
		delete m_StyleSheet;
		delete m_TextFormat;
		if (m_RootElement != nullptr)
		{
			delete m_RootElement;
		}
		delete m_State;
	}

	RtfTextFormat& RtfDomDocument::GetTextFormat()
	{
		return *m_TextFormat;
	}

	RtfWorld* RtfDomDocument::GetWorld() const
	{
		return m_World;
	}

	RtfDomElement* RtfDomDocument::GetRootElement() const
	{
		return m_RootElement;
	}

	RtfFontTable* RtfDomDocument::GetFontTable() const
	{
		return m_FontTable;
	}

	RtfColorTable* RtfDomDocument::GetColorTable() const
	{
		return m_ColorTable;
	}

	RtfStyleSheet* RtfDomDocument::GetStyleSheet() const
	{
		return m_StyleSheet;
	}

	bool RtfDomDocument::GetWidowControl() const
	{
		return m_WidowControl;
	}

	void RtfDomDocument::SetWidowControl(bool a_Value)
	{
		m_WidowControl = a_Value;
	}

	IRtfParseable::Result RtfDomDocument::_ParseCommand(RtfParserState& a_State, const RtfToken& a_Token)
	{
		IRtfParseable::Result result = IRtfParseable::eResult_Propagate;

		if (m_State->element != nullptr)
		{
			result = m_State->element->Parse(a_State, a_Token);
		}
		else
		{
			result = m_TextFormat->Parse(a_State, a_Token);
		}

		if (result != IRtfParseable::eResult_Propagate)
		{
			return result;
		}

		if (a_Token.value == "deff")
		{
			if (a_Token.parameter < 0)
			{
				return eResult_Invalid;
			}

			m_FontTable->SetDefault(a_Token.parameter);
			m_TextFormat->SetFont(m_FontTable->GetFont(a_Token.parameter));

			return eResult_Handled;
		}
		else if (a_Token.value == "pard")
		{
			// restore default paragraph parameters

			return eResult_Handled;
		}
		else if (a_Token.value == "par")
		{
			m_State->element = m_State->element->AddChild();

			return eResult_Handled;
		}
		else if (a_Token.value == "fonttbl")
		{
			a_State.target_current = m_FontTable;

			return a_State.target_current->Parse(a_State, a_Token);
		}
		else if (a_Token.value == "colortbl")
		{
			a_State.target_current = m_ColorTable;

			return a_State.target_current->Parse(a_State, a_Token);
		}
		else if (a_Token.value == "stylesheet")
		{
			a_State.target_current = m_StyleSheet;

			return a_State.target_current->Parse(a_State, a_Token);
		}

		return result;
	}

	IRtfParseable::Result RtfDomDocument::_ParseText(RtfParserState& a_State, const RtfToken& a_Token)
	{
		if (m_RootElement == nullptr)
		{
			m_RootElement = new RtfDomElement(*this);
			m_State->element = m_RootElement;
		}

		return m_State->element->Parse(a_State, a_Token);
	}

}; // namespace ExLibris