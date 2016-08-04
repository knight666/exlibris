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

namespace ExLibris {

	class FaceRequest
	{
	
	public:
		FaceRequest();

		bool HasFamilyName() const;
		const String& GetFamilyName() const;
		void SetFamilyName(const String& a_Name);

		bool HasSize() const;
		float GetSize() const;
		void SetSize(float a_Size);

		bool HasWeight() const;
		Weight GetWeight() const;
		void SetWeight(Weight a_Weight);

		bool HasStyle() const;
		Style GetStyle() const;
		void SetStyle(Style a_Style);
	
	private:
		String m_FamilyName;
		bool m_HasFamilyName;

		float m_Size;
		bool m_HasSize;

		Weight m_Weight;
		bool m_HasWeight;

		Style m_Style;
		bool m_HasStyle;
	
	};

};