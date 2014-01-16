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

	RtfDomDocument::RtfDomDocument(RtfWorld* a_World)
		: m_World(a_World)
		, m_TextFormat(nullptr)
		, m_WidowControl(true)
	{
		m_FontTable = new RtfFontTable(*this);
		m_ColorTable = new RtfColorTable(*this);
		m_StyleSheet = new RtfStyleSheet(*this);
		m_TextFormat = new RtfTextFormat(*this);
		m_RootElement = new RtfDomElement(*this);
	}

	RtfDomDocument::~RtfDomDocument()
	{
		delete m_FontTable;
		delete m_ColorTable;
		delete m_StyleSheet;
		delete m_TextFormat;
		delete m_RootElement;
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

}; // namespace ExLibris