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

#include <glm/glm.hpp>

namespace ExLibris
{

	class Bounds
	{
	
	public:
	
		Bounds();
		Bounds(const glm::vec2& a_Minimum, const glm::vec2& a_Maximum);
		Bounds(const Bounds& a_Other);

		// Properties

		const glm::vec2& GetMinimum() const;
		void SetMinimum(const glm::vec2& a_Minimum);

		const glm::vec2& GetMaximum() const;
		void SetMaximum(const glm::vec2& a_Maximum);

		glm::vec2 GetCenter() const;
		void SetCenter(const glm::vec2& a_Center);

		// Dimensions

		float GetWidth() const;
		void SetWidth(float a_Width);

		float GetHeight() const;
		void SetHeight(float a_Height);

		glm::vec2 GetDimensions() const;
		void SetDimensions(const glm::vec2& a_Dimensions);

		float GetHalfWidth() const;
		void SetHalfWidth(float a_HalfWidth);

		float GetHalfHeight() const;
		void SetHalfHeight(float a_HalfHeight);

		glm::vec2 GetHalfDimensions() const;
		void SetHalfDimensions(const glm::vec2& a_HalfDimensions);

		// Operators

		Bounds& operator = (const Bounds& a_Other);

		// Validity

		bool IsValid() const;
		void Invalidate();
		void Validate();
	
	private:

		glm::vec2 m_Minimum;
		glm::vec2 m_Maximum;
	
	}; // class Bounds

}; // namespace ExLibris