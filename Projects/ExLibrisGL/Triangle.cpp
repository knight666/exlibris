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

#include "Triangle.h"

namespace ExLibris
{

	Triangle::Triangle()
	{
	}

	Triangle::Triangle(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C)
		: a(a_A)
		, b(a_B)
		, c(a_C)
	{
	}

	bool Triangle::IsConvex() const
	{
		return (((a.y - b.y) * (c.x - b.x)) + ((b.x - a.x) * (c.y - b.y))) >= 0.0f;
	}

	bool Triangle::IsPositionInside(const glm::vec2& a_Position) const
	{
		glm::vec2 v0 = c - a;
		glm::vec2 v1 = b - a;
		glm::vec2 v2 = a_Position - a;

		float dot00 = glm::dot(v0, v0);
		float dot01 = glm::dot(v0, v1);
		float dot02 = glm::dot(v0, v2);
		float dot11 = glm::dot(v1, v1);
		float dot12 = glm::dot(v1, v2);

		float denominator = dot00 * dot11 - dot01 * dot01;

		float u = (dot11 * dot02 - dot01 * dot12) / denominator;
		if (u < 0.0f)
		{
			return false;
		}

		float v = (dot00 * dot12 - dot01 * dot02) / denominator;
		if (v < 0.0f)
		{
			return false;
		}

		return (u + v) < 1.0f;
	}

}; // namespace ExLibris