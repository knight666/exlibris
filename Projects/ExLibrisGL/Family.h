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

#include "Base.h"

#include "Style.h"
#include "Weight.h"

namespace ExLibris
{
	class IGlyphProvider;
	class Library;
};

namespace ExLibris {

	class Family
	{
	
	public:
		Family(Library* a_Library, const String& a_Name);
		~Family();
	
		Library* GetLibrary() const;

		const String& GetName() const;

		size_t GetGlyphProviderCount() const;
		void AddGlyphProvider(IGlyphProvider* a_Provider);
		IGlyphProvider* FindGlyphProvider(float a_Size, Weight a_Weight, Style a_Style) const;

	private:

		Library* m_Library;
		String m_Name;
		std::set<IGlyphProvider*> m_GlyphProviders;
	
	};

};