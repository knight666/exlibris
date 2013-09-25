#include "ExLibrisGL.PCH.h"

#include "MeshBuilder.h"

namespace ExLibris
{

	void MeshBuilder::Clear()
	{
		m_DataTypes.clear();
		m_DataPositions.clear();
	}

	void MeshBuilder::AddTriangle(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C)
	{
		m_DataTypes.push_back(eDataType_Triangle);

		m_DataPositions.push_back(a_A);
		m_DataPositions.push_back(a_B);
		m_DataPositions.push_back(a_C);
	}

	void MeshBuilder::AddQuad(const glm::vec2& a_UpperLeft, const glm::vec2& a_UpperRight, const glm::vec2& a_LowerLeft, const glm::vec2& a_LowerRight)
	{
		m_DataTypes.push_back(eDataType_Quad);

		m_DataPositions.push_back(a_UpperLeft);
		m_DataPositions.push_back(a_UpperRight);
		m_DataPositions.push_back(a_LowerLeft);
		m_DataPositions.push_back(a_LowerRight);
	}

	void MeshBuilder::Accept(IMeshVisitor& a_Visitor) const
	{
		if (m_DataTypes.size() == 0)
		{
			return;
		}

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

					a_Visitor.VisitBuilderAddTriangle(a, b, c);

				} break;

			case eDataType_Quad:
				{
					const glm::vec2& ul = *position_it++;
					const glm::vec2& ur = *position_it++;
					const glm::vec2& ll = *position_it++;
					const glm::vec2& lr = *position_it++;

					a_Visitor.VisitBuilderAddTriangle(ur, ul, ll);
					a_Visitor.VisitBuilderAddTriangle(ur, ll, lr);

				} break;

			}
		}
	}

}; // namespace ExLibris