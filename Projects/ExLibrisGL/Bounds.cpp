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
		: m_Minimum(std::numeric_limits<float>::max(), std::numeric_limits<float>::max())
		, m_Maximum(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max())
	{
	}

	Bounds::Bounds(const glm::vec2& a_Minimum, const glm::vec2& a_Maximum)
		: m_Minimum(a_Minimum)
		, m_Maximum(a_Maximum)
	{
	}

	const glm::vec2& Bounds::GetMinimum() const
	{
		return m_Minimum;
	}

	const glm::vec2& Bounds::GetMaximum() const
	{
		return m_Maximum;
	}

	bool Bounds::IsValid() const
	{
		return ((m_Minimum.x <= m_Maximum.x) && (m_Minimum.y <= m_Maximum.y));
	}

}; // namespace ExLibris