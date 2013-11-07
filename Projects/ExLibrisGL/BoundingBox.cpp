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

#include "BoundingBox.h"

namespace ExLibris
{

	BoundingBox::BoundingBox()
	{
		Invalidate();
	}

	BoundingBox::BoundingBox(const glm::vec2& a_Minimum, const glm::vec2& a_Maximum)
		: m_Minimum(a_Minimum)
		, m_Maximum(a_Maximum)
	{
		Validate();
	}

	BoundingBox::BoundingBox(const BoundingBox& a_Other)
		: m_Minimum(a_Other.GetMinimum())
		, m_Maximum(a_Other.GetMaximum())
	{
	}

	// Properties

	const glm::vec2& BoundingBox::GetMinimum() const
	{
		return m_Minimum;
	}

	void BoundingBox::SetMinimum(const glm::vec2& a_Minimum)
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

	const glm::vec2& BoundingBox::GetMaximum() const
	{
		return m_Maximum;
	}

	void BoundingBox::SetMaximum(const glm::vec2& a_Maximum)
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

	glm::vec2 BoundingBox::GetCenter() const
	{
		return (m_Minimum + m_Maximum) / 2.0f;
	}

	void BoundingBox::SetCenter(const glm::vec2& a_Center)
	{
		glm::vec2 half_dimensions = GetHalfDimensions();

		m_Minimum = a_Center - half_dimensions;
		m_Maximum = a_Center + half_dimensions;
	}

	// Corners

	float BoundingBox::GetLeft() const
	{
		return m_Minimum.x;
	}

	void BoundingBox::SetLeft(float a_Left)
	{
		if (m_Minimum.x < m_Maximum.x)
		{
			m_Minimum.x = a_Left;

			if (m_Minimum.x > m_Maximum.x)
			{
				float swap = m_Maximum.x;
				m_Maximum.x = m_Minimum.x;
				m_Minimum.x = swap;
			}
		}
		else
		{
			m_Minimum.x = m_Maximum.x = a_Left;
		}
	}

	float BoundingBox::GetRight() const
	{
		return m_Maximum.x;
	}

	void BoundingBox::SetRight(float a_Right)
	{
		if (m_Minimum.x < m_Maximum.x)
		{
			m_Maximum.x = a_Right;

			if (m_Minimum.x > m_Maximum.x)
			{
				float swap = m_Maximum.x;
				m_Maximum.x = m_Minimum.x;
				m_Minimum.x = swap;
			}
		}
		else
		{
			m_Minimum.x = m_Maximum.x = a_Right;
		}
	}

	float BoundingBox::GetTop() const
	{
		return m_Minimum.y;
	}

	void BoundingBox::SetTop(float a_Top)
	{
		if (m_Minimum.y < m_Maximum.y)
		{
			m_Minimum.y = a_Top;

			if (m_Minimum.y > m_Maximum.y)
			{
				float swap = m_Maximum.y;
				m_Maximum.y = m_Minimum.y;
				m_Minimum.y = swap;
			}
		}
		else
		{
			m_Minimum.y = m_Maximum.y = a_Top;
		}
	}

	float BoundingBox::GetBottom() const
	{
		return m_Maximum.y;
	}

	void BoundingBox::SetBottom(float a_Bottom)
	{
		if (m_Minimum.y < m_Maximum.y)
		{
			m_Maximum.y = a_Bottom;

			if (m_Minimum.y > m_Maximum.y)
			{
				float swap = m_Maximum.y;
				m_Maximum.y = m_Minimum.y;
				m_Minimum.y = swap;
			}
		}
		else
		{
			m_Minimum.y = m_Maximum.y = a_Bottom;
		}
	}

	glm::vec2 BoundingBox::GetTopLeft() const
	{
		return m_Minimum;
	}

	void BoundingBox::SetTopLeft(const glm::vec2& a_TopLeft)
	{
		if (IsValid())
		{
			m_Minimum = a_TopLeft;

			Validate();
		}
		else
		{
			m_Minimum = a_TopLeft;
			m_Maximum = a_TopLeft;
		}
	}

	glm::vec2 BoundingBox::GetTopRight() const
	{
		return glm::vec2(m_Maximum.x, m_Minimum.y);
	}

	void BoundingBox::SetTopRight(const glm::vec2& a_TopRight)
	{
		if (IsValid())
		{
			m_Maximum.x = a_TopRight.x;
			m_Minimum.y = a_TopRight.y;

			Validate();
		}
		else
		{
			m_Minimum = a_TopRight;
			m_Maximum = a_TopRight;
		}
	}

	glm::vec2 BoundingBox::GetBottomLeft() const
	{
		return glm::vec2(m_Minimum.x, m_Maximum.y);
	}

	void BoundingBox::SetBottomLeft(const glm::vec2& a_BottomLeft)
	{
		if (IsValid())
		{
			m_Minimum.x = a_BottomLeft.x;
			m_Maximum.y = a_BottomLeft.y;

			Validate();
		}
		else
		{
			m_Minimum = a_BottomLeft;
			m_Maximum = a_BottomLeft;
		}
	}

	glm::vec2 BoundingBox::GetBottomRight() const
	{
		return m_Maximum;
	}

	void BoundingBox::SetBottomRight(const glm::vec2& a_BottomRight)
	{
		if (IsValid())
		{
			m_Maximum = a_BottomRight;

			Validate();
		}
		else
		{
			m_Minimum = a_BottomRight;
			m_Maximum = a_BottomRight;
		}
	}

	// Dimensions

	float BoundingBox::GetWidth() const
	{
		return IsValid() ? glm::abs(m_Maximum.x - m_Minimum.x) : 0.0f;
	}

	void BoundingBox::SetWidth(float a_Width)
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

	float BoundingBox::GetHeight() const
	{
		return IsValid() ? glm::abs(m_Maximum.y - m_Minimum.y) : 0.0f;
	}

	void BoundingBox::SetHeight(float a_Height)
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

	glm::vec2 BoundingBox::GetDimensions() const
	{
		return IsValid() ? glm::abs(m_Maximum - m_Minimum) : glm::vec2(0.0f, 0.0f);
	}

	void BoundingBox::SetDimensions(const glm::vec2& a_Dimensions)
	{
		if (!IsValid())
		{
			m_Minimum = glm::vec2(0.0f, 0.0f);
		}

		m_Maximum = m_Minimum + a_Dimensions;

		Validate();
	}

	float BoundingBox::GetHalfWidth() const
	{
		return IsValid() ? (glm::abs(m_Maximum.x - m_Minimum.x) / 2.0f) : 0.0f;
	}

	void BoundingBox::SetHalfWidth(float a_HalfWidth)
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

	float BoundingBox::GetHalfHeight() const
	{
		return IsValid() ? (glm::abs(m_Maximum.y - m_Minimum.y) / 2.0f) : 0.0f;
	}

	void BoundingBox::SetHalfHeight(float a_HalfHeight)
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

	glm::vec2 BoundingBox::GetHalfDimensions() const
	{
		return IsValid() ? (glm::abs(m_Maximum - m_Minimum) / 2.0f) : glm::vec2(0.0f, 0.0f);
	}

	void BoundingBox::SetHalfDimensions(const glm::vec2& a_HalfDimensions)
	{
		glm::vec2 center = GetCenter();

		m_Minimum = center - a_HalfDimensions;
		m_Maximum = center + a_HalfDimensions;

		Validate();
	}

	// Operators

	BoundingBox& BoundingBox::operator = (const BoundingBox& a_Other)
	{
		m_Minimum = a_Other.GetMinimum();
		m_Maximum = a_Other.GetMaximum();

		return *this;
	}

	// Translation

	void BoundingBox::Translate(const glm::vec2& a_Offset)
	{
		m_Minimum += a_Offset;
		m_Maximum += a_Offset;
	}

	BoundingBox BoundingBox::GetTranslated(const glm::vec2& a_Offset) const
	{
		return BoundingBox(
			m_Minimum + a_Offset,
			m_Maximum + a_Offset
		);
	}

	// Intersection

	bool BoundingBox::IsIntersected(const BoundingBox& a_Other) const
	{
		if (!IsValid() || !a_Other.IsValid())
		{
			return false;
		}

		glm::vec2 difference = glm::abs(GetCenter() - a_Other.GetCenter());
		glm::vec2 half_dimensions = GetHalfDimensions() + a_Other.GetHalfDimensions();

		return (
			difference.x <= half_dimensions.x && 
			difference.y <= half_dimensions.y
		);
	}

	void BoundingBox::Intersect(const BoundingBox& a_Other)
	{
		if (IsIntersected(a_Other))
		{
			glm::vec2 clamped_minimum = glm::clamp(a_Other.GetMinimum(), m_Minimum, m_Maximum);
			glm::vec2 clamped_maximum = glm::clamp(a_Other.GetMaximum(), m_Minimum, m_Maximum);

			m_Minimum = clamped_minimum;
			m_Maximum = clamped_maximum;
		}
	}

	BoundingBox BoundingBox::GetIntersection(const BoundingBox& a_Other) const
	{
		if (!IsIntersected(a_Other))
		{
			return BoundingBox();
		}

		BoundingBox intersection(
			glm::clamp(a_Other.GetMinimum(), m_Minimum, m_Maximum),
			glm::clamp(a_Other.GetMaximum(), m_Minimum, m_Maximum)
		);

		return intersection;
	}

	bool BoundingBox::IsContained(const BoundingBox& a_Other) const
	{
		if (!IsValid() || !a_Other.IsValid())
		{
			return false;
		}

		return (
			(a_Other.GetMinimum().x >= m_Minimum.x && a_Other.GetMinimum().x <= m_Maximum.x) &&
			(a_Other.GetMinimum().y >= m_Minimum.y && a_Other.GetMinimum().y <= m_Maximum.y) &&
			(a_Other.GetMaximum().x >= m_Minimum.x && a_Other.GetMaximum().x <= m_Maximum.x) &&
			(a_Other.GetMaximum().y >= m_Minimum.y && a_Other.GetMaximum().y <= m_Maximum.y)
		);
	}

	void BoundingBox::Unite(const BoundingBox& a_Other)
	{
		m_Minimum = glm::min(m_Minimum, a_Other.GetMinimum());
		m_Maximum = glm::max(m_Maximum, a_Other.GetMaximum());
	}

	BoundingBox BoundingBox::GetUnited(const BoundingBox& a_Other) const
	{
		return BoundingBox(
			glm::min(m_Minimum, a_Other.GetMinimum()),
			glm::max(m_Maximum, a_Other.GetMaximum())
		);
	}

	// Validity

	bool BoundingBox::IsValid() const
	{
		return ((m_Minimum.x <= m_Maximum.x) && (m_Minimum.y <= m_Maximum.y));
	}

	void BoundingBox::Invalidate()
	{
		m_Minimum.x = std::numeric_limits<float>::max();
		m_Minimum.y = std::numeric_limits<float>::max();

		m_Maximum.x = -std::numeric_limits<float>::max();
		m_Maximum.y = -std::numeric_limits<float>::max();
	}

	void BoundingBox::Validate()
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