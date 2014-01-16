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

#include "RtfColor.h"

namespace ExLibris
{
	class RtfDomDocument;
}

namespace ExLibris
{

	class RtfColorTable
	{

	public:

		RtfColorTable(RtfDomDocument& a_Document);
		~RtfColorTable();

		size_t GetColorCount() const;

		RtfColor* GetColor(int a_Index);

		RtfColor* AddColor(const RtfColor& a_Color);

		RtfColor* GetDefaultColor() const;
		void SetDefaultColor(int a_Index);

	private:

		RtfDomDocument& m_Document;
		std::map<int, RtfColor*> m_Colors;
		RtfColor* m_ColorDefault;
		int m_IndexNext;
		int m_IndexDefault;

	}; // class RtfColorTable

}; // namespace ExLibris