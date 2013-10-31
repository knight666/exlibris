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

#include "Bounds.h"

namespace ExLibris
{

	Bounds::Bounds()
	{
		Invalidate();
	}

	Bounds::Bounds(const glm::vec2& a_Minimum, const glm::vec2& a_Maximum)
		: m_Minimum(a_Minimum)
		, m_Maximum(a_Maximum)
	{
		Validate();
	}

	Bounds::Bounds(const Bounds& a_Other)
		: m_Minimum(a_Other.GetMinimum())
		, m_Maximum(a_Other.GetMaximum())
	{
	}

	// Properties

	const glm::vec2& Bounds::GetMinimum() const
	{
		return m_Minimum;
	}

	void Bounds::SetMinimum(const glm::vec2& a_Minimum)
	{
		if (IsValid())
		{
			m_Minimum = a_Minimum;

			Validate();
		}
		else
		{
			m_Minimum = a_Minimum;
			m_Maximum = a_Minimum;
		}
	}

	const glm::vec2& Bounds::GetMaximum() const
	{
		return m_Maximum;
	}

	void Bounds::SetMaximum(const glm::vec2& a_Maximum)
	{
		if (IsValid())
		{
			m_Maximum = a_Maximum;

			Validate();
		}
		else
		{
			m_Minimum = a_Maximum;
			m_Maximum = a_Maximum;
		}
	}

	glm::vec2 Bounds::GetCenter() const
	{
		return (m_Minimum + m_Maximum) / 2.0f;
	}

	void Bounds::SetCenter(const glm::vec2& a_Center)
	{
		glm::vec2 half_dimensions = GetHalfDimensions();

		m_Minimum = a_Center - half_dimensions;
		m_Maximum = a_Center + half_dimensions;
	}

	// Dimensions

	float Bounds::GetWidth() const
	{
		return IsValid() ? glm::abs(m_Maximum.x - m_Minimum.x) : 0.0f;
	}

	void Bounds::SetWidth(float a_Width)
	{
		if (m_Minimum.x > m_Maximum.x)
		{
			m_Minimum.x = 0.0f;
		}

		m_Maximum.x = m_Minimum.x + a_Width;

		if (m_Minimum.x > m_Maximum.x)
		{
			float swap = m_Maximum.x;
			m_Maximum.x = m_Minimum.x;
			m_Minimum.x = swap;
		}
	}

	float Bounds::GetHeight() const
	{
		return IsValid() ? glm::abs(m_Maximum.y - m_Minimum.y) : 0.0f;
	}

	void Bounds::SetHeight(float a_Height)
	{
		if (m_Minimum.y > m_Maximum.y)
		{
			m_Minimum.y = 0.0f;
		}

		m_Maximum.y = m_Minimum.y + a_Height;

		if (m_Minimum.y > m_Maximum.y)
		{
			float swap = m_Maximum.y;
			m_Maximum.y = m_Minimum.y;
			m_Minimum.y = swap;
		}
	}

	glm::vec2 Bounds::GetDimensions() const
	{
		return IsValid() ? glm::abs(m_Maximum - m_Minimum) : glm::vec2(0.0f, 0.0f);
	}

	void Bounds::SetDimensions(const glm::vec2& a_Dimensions)
	{
		if (!IsValid())
		{
			m_Minimum = glm::vec2(0.0f, 0.0f);
		}

		m_Maximum = m_Minimum + a_Dimensions;

		Validate();
	}

	float Bounds::GetHalfWidth() const
	{
		return IsValid() ? (glm::abs(m_Maximum.x - m_Minimum.x) / 2.0f) : 0.0f;
	}

	void Bounds::SetHalfWidth(float a_HalfWidth)
	{
		float center_x  = (m_Minimum.x + m_Maximum.x) / 2.0f;

		m_Minimum.x = center_x - a_HalfWidth;
		m_Maximum.x = center_x + a_HalfWidth;

		if (m_Minimum.x > m_Maximum.x)
		{
			float swap = m_Maximum.x;
			m_Maximum.x = m_Minimum.x;
			m_Minimum.x = swap;
		}
	}

	float Bounds::GetHalfHeight() const
	{
		return IsValid() ? (glm::abs(m_Maximum.y - m_Minimum.y) / 2.0f) : 0.0f;
	}

	void Bounds::SetHalfHeight(float a_HalfHeight)
	{
		float center_y  = (m_Minimum.y + m_Maximum.y) / 2.0f;

		m_Minimum.y = center_y - a_HalfHeight;
		m_Maximum.y = center_y + a_HalfHeight;

		if (m_Minimum.y > m_Maximum.y)
		{
			float swap = m_Maximum.y;
			m_Maximum.y = m_Minimum.y;
			m_Minimum.y = swap;
		}
	}

	glm::vec2 Bounds::GetHalfDimensions() const
	{
		return IsValid() ? (glm::abs(m_Maximum - m_Minimum) / 2.0f) : glm::vec2(0.0f, 0.0f);
	}

	void Bounds::SetHalfDimensions(const glm::vec2& a_HalfDimensions)
	{
		glm::vec2 center = GetCenter();

		m_Minimum = center - a_HalfDimensions;
		m_Maximum = center + a_HalfDimensions;

		Validate();
	}

	// Operators

	Bounds& Bounds::operator = (const Bounds& a_Other)
	{
		m_Minimum = a_Other.GetMinimum();
		m_Maximum = a_Other.GetMaximum();

		return *this;
	}

	// Validity

	bool Bounds::IsValid() const
	{
		return ((m_Minimum.x <= m_Maximum.x) && (m_Minimum.y <= m_Maximum.y));
	}

	void Bounds::Invalidate()
	{
		m_Minimum.x = std::numeric_limits<float>::max();
		m_Minimum.y = std::numeric_limits<float>::max();

		m_Maximum.x = -std::numeric_limits<float>::max();
		m_Maximum.y = -std::numeric_limits<float>::max();
	}

	void Bounds::Validate()
	{
		if (m_Minimum.x > m_Maximum.x)
		{
			float swap = m_Maximum.x;
			m_Maximum.x = m_Minimum.x;
			m_Minimum.x = swap;
		}

		if (m_Minimum.y > m_Maximum.y)
		{
			float swap = m_Maximum.y;
			m_Maximum.y = m_Minimum.y;
			m_Minimum.y = swap;
		}
	}

}; // namespace ExLibris