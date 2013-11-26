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

#include "BoundingBox.h"
#include "ITextLayoutVisitor.h"

namespace ExLibris
{

	class ITextLayoutElement
	{
	
	public:

		ITextLayoutElement()
			: m_ElementGeometry(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f))
			, m_LayoutGeometry(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f))
		{
		}

		virtual ~ITextLayoutElement()
		{
		}
	
	public:

		const glm::mat3x3& GetLocalTransform() const
		{
			return m_TransformLocal;
		}

		const BoundingBox& GetElementGeometry() const
		{
			return m_ElementGeometry;
		}

		const BoundingBox& GetLayoutGeometry() const
		{
			return m_LayoutGeometry;
		}

		void AddChild(ITextLayoutElement* a_Child)
		{
			m_Children.push_back(a_Child);
		}

		void CalculateGeometry()
		{
			m_ElementGeometry.Invalidate();
			m_LayoutGeometry.Invalidate();

			for (std::vector<ITextLayoutElement*>::iterator child_it = m_Children.begin(); child_it != m_Children.end(); ++child_it)
			{
				ITextLayoutElement* child = *child_it;

				child->CalculateGeometry();

				m_ElementGeometry.Unite(child->GetElementGeometry());
				m_LayoutGeometry.Unite(child->GetLayoutGeometry());
			}

			OnGeometryCalculated();
		}

		void Accept(ITextLayoutVisitor& a_Visitor)
		{
			OnAccepted(a_Visitor);

			for (std::vector<ITextLayoutElement*>::iterator child_it = m_Children.begin(); child_it != m_Children.end(); ++child_it)
			{
				ITextLayoutElement* child = *child_it;

				child->Accept(a_Visitor);
			}
		}

	protected:

		virtual void OnGeometryCalculated() = 0;
		virtual void OnAccepted(ITextLayoutVisitor& a_Visitor) = 0;

	protected:

		std::vector<ITextLayoutElement*> m_Children;

		BoundingBox m_ElementGeometry;
		BoundingBox m_LayoutGeometry;
		glm::mat3x3 m_TransformLocal;
		glm::mat3x3 m_TransformGlobal;
	
	}; // class ITextLayoutElement

}; // namespace ExLibris