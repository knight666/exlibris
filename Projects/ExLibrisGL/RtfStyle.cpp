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

#include "RtfStyle.h"

namespace ExLibris
{

	RtfStyle::RtfStyle()
	{
		m_StyleNextParagraph = this;

		m_PropertiesLowAnsi = new RtfAssociatedProperties();
		m_PropertiesLowAnsi->SetCharacterEncoding(eRtfCharacterEncoding_SingleByteLowAnsi);

		m_PropertiesHighAnsi = new RtfAssociatedProperties();
		m_PropertiesHighAnsi->SetCharacterEncoding(eRtfCharacterEncoding_SingleByteHighAnsi);

		m_PropertiesDoubleByte = new RtfAssociatedProperties();
		m_PropertiesDoubleByte->SetCharacterEncoding(eRtfCharacterEncoding_DoubleByte);
	}

	RtfStyle::~RtfStyle()
	{
		delete m_PropertiesLowAnsi;
		delete m_PropertiesHighAnsi;
		delete m_PropertiesDoubleByte;
	}

	const std::string& RtfStyle::GetName() const
	{
		return m_Name;
	}

	void RtfStyle::SetName(const std::string& a_Name)
	{
		m_Name = a_Name;
	}

	RtfStyle* RtfStyle::GetNextParagraphStyle() const
	{
		return m_StyleNextParagraph;
	}

	void RtfStyle::SetNextParagraphStyle(RtfStyle* a_Style)
	{
		m_StyleNextParagraph = a_Style;
	}

	RtfAssociatedProperties* RtfStyle::GetPropertiesForCharacterEncoding(RtfCharacterEncoding a_Encoding)
	{
		switch (a_Encoding)
		{

		case eRtfCharacterEncoding_SingleByteLowAnsi:
			{
				return m_PropertiesLowAnsi;

			} break;

		case eRtfCharacterEncoding_SingleByteHighAnsi:
			{
				return m_PropertiesHighAnsi;

			} break;

		case eRtfCharacterEncoding_DoubleByte:
			{
				return m_PropertiesDoubleByte;

			} break;
		}

		return nullptr;
	}

	RtfAssociatedProperties RtfStyle::GetCombinedPropertiesForCharacterEncoding(RtfCharacterEncoding a_Encoding)
	{
		RtfAssociatedProperties combined(m_TextFormat);

		RtfAssociatedProperties* properties = GetPropertiesForCharacterEncoding(a_Encoding);
		if (properties != nullptr)
		{
			combined.Combine(*properties);
		}

		return combined;
	}

	RtfTextFormat& RtfStyle::GetTextFormat()
	{
		return m_TextFormat;
	}

}; // namespace ExLibris