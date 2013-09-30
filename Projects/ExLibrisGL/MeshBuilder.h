#ifndef _MESHBUILDER_H_
#define _MESHBUILDER_H_

#include "IMeshVisitor.h"

#include <vector>
#include <glm/glm.hpp>

namespace ExLibris
{

	class MeshBuilder
	{
	
	public:

		MeshBuilder();

		size_t GetVertexCount() const;

		void Clear();

		void AddTriangle(
			const glm::vec2& a_A,
			const glm::vec2& a_B,
			const glm::vec2& a_C
		);

		void AddQuad(
			const glm::vec2& a_UpperLeft, const glm::vec2& a_UpperRight,
			const glm::vec2& a_LowerLeft, const glm::vec2& a_LowerRight
		);

		void Accept(IMeshVisitor& a_Visitor) const;

	private:

		enum DataType
		{
			eDataType_Triangle,
			eDataType_Quad
		};

		size_t m_VertexCount;
		std::vector<DataType> m_DataTypes;
		std::vector<glm::vec2> m_DataPositions;
	
	}; // class MeshBuilder

}; // namespace ExLibris

#endif