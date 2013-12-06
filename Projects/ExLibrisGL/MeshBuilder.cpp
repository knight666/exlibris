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

#include "MeshBuilder.h"

namespace ExLibris
{

	MeshBuilder::MeshBuilder()
		: m_VertexCount(0)
	{
	}

	size_t MeshBuilder::GetVertexCount() const
	{
		return m_VertexCount;
	}

	void MeshBuilder::Clear()
	{
		m_VertexCount = 0;
		m_DataTypes.clear();
		m_DataPositions.clear();
	}

	void MeshBuilder::AddTriangle(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C)
	{
		m_DataTypes.push_back(eDataType_Triangle);

		m_DataPositions.push_back(a_A);
		m_DataPositions.push_back(a_B);
		m_DataPositions.push_back(a_C);

		m_VertexCount += 3;
	}

	void MeshBuilder::AddQuad(const glm::vec2& a_UpperLeft, const glm::vec2& a_UpperRight, const glm::vec2& a_LowerLeft, const glm::vec2& a_LowerRight)
	{
		m_DataTypes.push_back(eDataType_Quad);

		m_DataPositions.push_back(a_UpperLeft);
		m_DataPositions.push_back(a_UpperRight);
		m_DataPositions.push_back(a_LowerLeft);
		m_DataPositions.push_back(a_LowerRight);

		m_VertexCount += 6;
	}

	void MeshBuilder::Accept(IMeshVisitor& a_Visitor) const
	{
		if (m_DataTypes.size() == 0)
		{
			return;
		}

		a_Visitor.VisitBuilderMeshBegin(m_VertexCount);

		std::vector<glm::vec2>::const_iterator position_it = m_DataPositions.begin();

		for (std::vector<DataType>::const_iterator type_it = m_DataTypes.begin(); type_it != m_DataTypes.end(); ++type_it)
		{
			DataType type = *type_it;

			switch (*type_it)
			{

			case eDataType_Triangle:
				{
					const glm::vec2& a = *position_it++;
					const glm::vec2& b = *position_it++;
					const glm::vec2& c = *position_it++;

					a_Visitor.VisitBuilderTriangle(a, b, c);

				} break;

			case eDataType_Quad:
				{
					const glm::vec2& ul = *position_it++;
					const glm::vec2& ur = *position_it++;
					const glm::vec2& ll = *position_it++;
					const glm::vec2& lr = *position_it++;

					a_Visitor.VisitBuilderTriangle(ur, ul, ll);
					a_Visitor.VisitBuilderTriangle(ur, ll, lr);

				} break;

			}
		}

		a_Visitor.VisitBuilderMeshEnd();
	}

}; // namespace ExLibris