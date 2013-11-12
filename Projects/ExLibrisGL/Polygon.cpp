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

#include "Polygon.h"

namespace ExLibris
{

	Polygon& Polygon::operator += (const glm::vec2& a_Position)
	{
		positions.push_back(a_Position);

		return *this;
	}

	bool Polygon::IsPositionInside(const glm::vec2& a_Position) const
	{
		if (positions.size() < 3)
		{
			return false;
		}

		bool inside = false;

		std::vector<glm::vec2>::const_iterator start_it = positions.begin();
		std::vector<glm::vec2>::const_iterator end_it = positions.end() - 1;

		while (start_it != positions.end())
		{
			const glm::vec2& start = *start_it;
			const glm::vec2& end = *end_it;

			if ((start.y > a_Position.y) != (end.y > a_Position.y))
			{
				bool raycast = (a_Position.x < ((end.x - start.x) * (a_Position.y - start.y) / (end.y - start.y) + start.x));
				if (raycast)
				{
					inside = !inside;
				}
			}

			end_it = start_it++;
		}

		return inside;
	}

	bool Polygon::Intersects(const Polygon& a_Other) const
	{
		if (positions.size() < 3 || a_Other.positions.size() < 3)
		{
			return false;
		}

		for (std::vector<glm::vec2>::const_iterator position_it = a_Other.positions.begin(); position_it != a_Other.positions.end(); ++position_it)
		{
			if (IsPositionInside(*position_it))
			{
				return true;
			}
		}

		return false;
	}

	BoundingBox Polygon::GetBoundingBox() const
	{
		if (positions.size() == 0)
		{
			return BoundingBox();
		}

		BoundingBox bounds;

		for (std::vector<glm::vec2>::const_iterator position_it = positions.begin(); position_it != positions.end(); ++position_it)
		{
			bounds.ExpandToFit(*position_it);
		}

		return bounds;
	}

}; // namespace ExLibris