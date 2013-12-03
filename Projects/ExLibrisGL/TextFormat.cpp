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

#include "TextFormat.h"

#include "Face.h"
#include "Library.h"

namespace ExLibris
{

	TextFormat::TextFormat(Library* a_Library)
		: m_Library(a_Library)
		, m_Family(nullptr)
		, m_Face(nullptr)
		, m_FaceDirty(true)
		, m_Properties(0)
		, m_Color(0.0f, 0.0f, 0.0f, 1.0f)
		, m_BackgroundColor(1.0f, 1.0f, 1.0f, 1.0f)
	{
	}

	TextFormat::~TextFormat()
	{
	}

	Face* TextFormat::GetFace()
	{
		if (m_FaceDirty)
		{
			_LoadFace();

			m_FaceDirty = false;
		}

		return m_Face;
	}

	Family* TextFormat::GetFamily()
	{
		if (m_FaceDirty)
		{
			_LoadFace();

			m_FaceDirty = false;
		}

		return m_Family;
	}

	bool TextFormat::HasProperty(unsigned int a_Property) const
	{
		return ((m_Properties & a_Property) != 0);
	}

	const std::string& TextFormat::GetFamilyName() const
	{
		return m_Request.GetFamilyName();
	}

	void TextFormat::SetFamilyName(const std::string& a_Name)
	{
		m_Request.SetFamilyName(a_Name);
		m_Properties |= eProperty_FamilyName;
		m_FaceDirty = true;
	}

	float TextFormat::GetSize() const
	{
		return m_Request.GetSize();
	}

	void TextFormat::SetSize(float a_Size)
	{
		m_Request.SetSize(a_Size);
		m_Properties |= eProperty_Size;
		m_FaceDirty = true;
	}

	Weight TextFormat::GetWeight() const
	{
		return m_Request.GetWeight();
	}

	void TextFormat::SetWeight(Weight a_Weight)
	{
		m_Request.SetWeight(a_Weight);
		m_Properties |= eProperty_Weight;
		m_FaceDirty = true;
	}

	Style TextFormat::GetStyle() const
	{
		return m_Request.GetStyle();
	}

	void TextFormat::SetStyle(Style a_Style)
	{
		m_Request.SetStyle(a_Style);
		m_Properties |= eProperty_Style;
		m_FaceDirty = true;
	}

	const glm::vec4& TextFormat::GetColor() const
	{
		return m_Color;
	}

	void TextFormat::SetColor(const glm::vec4& a_Color)
	{
		m_Color = a_Color;
		m_Properties |= eProperty_Color;
	}

	const glm::vec4& TextFormat::GetBackgroundColor() const
	{
		return m_BackgroundColor;
	}

	void TextFormat::SetBackgroundColor(const glm::vec4& a_Color)
	{
		m_BackgroundColor = a_Color;
		m_Properties |= eProperty_BackgroundColor;
	}

	void TextFormat::_LoadFace()
	{
		m_Face = m_Library->RequestFace(m_Request);
		if (m_Face != nullptr)
		{
			m_Family = m_Face->GetFamily();
		}
	}

}; // namespace ExLibris