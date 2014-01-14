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

#include "RtfAssociatedProperties.h"
#include "RtfTextFormat.h"

namespace ExLibris
{
	struct RtfColor;
	struct RtfLocale;
}

namespace ExLibris
{

	class RtfStyle
	{

	public:

		RtfStyle();
		~RtfStyle();

		const std::string& GetName() const;
		void SetName(const std::string& a_Name);

		RtfStyle* GetNextParagraphStyle() const;
		void SetNextParagraphStyle(RtfStyle* a_Style);

		RtfAssociatedProperties* GetPropertiesForCharacterEncoding(RtfCharacterEncoding a_Encoding);
		RtfAssociatedProperties GetCombinedPropertiesForCharacterEncoding(RtfCharacterEncoding a_Encoding);

		RtfTextFormat& GetTextFormat();

	private:

		std::string m_Name;
		RtfStyle* m_StyleNextParagraph;
		RtfTextFormat m_TextFormat;
		RtfAssociatedProperties* m_PropertiesLowAnsi;
		RtfAssociatedProperties* m_PropertiesHighAnsi;
		RtfAssociatedProperties* m_PropertiesDoubleByte;

	}; // class RtfStyle

}; // namespace ExLibris