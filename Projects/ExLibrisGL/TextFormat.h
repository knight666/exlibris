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

#include "FaceRequest.h"
#include "Style.h"
#include "Weight.h"

namespace ExLibris
{
	class Face;
	class Family;
	class Library;
}

namespace ExLibris
{

	class TextFormat
	{
	
	public:

		enum Property
		{
			eProperty_FamilyName      = (1 << 0),
			eProperty_Size            = (1 << 1),
			eProperty_Weight          = (1 << 2),
			eProperty_Style           = (1 << 3),
			eProperty_Color           = (1 << 4),
			eProperty_BackgroundColor = (1 << 5)
		};

	public:
	
		TextFormat(Library* a_Library);
		~TextFormat();

		Face* GetFace();
		void SetFace(Face* a_Face);

		Family* GetFamily();

		bool HasProperty(unsigned int a_Property) const;

		const std::string& GetFamilyName() const;
		void SetFamilyName(const std::string& a_Name);

		float GetSize() const;
		void SetSize(float a_Size);

		Weight GetWeight() const;
		void SetWeight(Weight a_Weight);

		Style GetStyle() const;
		void SetStyle(Style a_Style);

		const glm::vec4& GetColor() const;
		void SetColor(const glm::vec4& a_Color);

		const glm::vec4& GetBackgroundColor() const;
		void SetBackgroundColor(const glm::vec4& a_Color);

	private:

		void _LoadFace();

	private:

		Library* m_Library;
		Face* m_Face;
		bool m_FaceDirty;
		Family* m_Family;

		unsigned int m_Properties;

		FaceRequest m_Request;

		glm::vec4 m_Color;
		glm::vec4 m_BackgroundColor;
	
	}; // class TextFormat

}; // namespace ExLibris