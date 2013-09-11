#ifndef _SHAPEPATH_H_
#define _SHAPEPATH_H_

#include "IShapePathVisitor.h"

#include <glm/glm.hpp>
#include <vector>

namespace ExLibris
{

	class ShapePath
	{
	
	public:
	
		ShapePath();
		~ShapePath();

		void Move(const glm::vec2& a_Position);
		void LineTo(const glm::vec2& a_To);
		void ConicCurveTo(const glm::vec2& a_Control, const glm::vec2& a_To);
		void QuadraticCurveTo(const glm::vec2& a_ControlA, const glm::vec2& a_ControLB, const glm::vec2& a_To);

		void Accept(IShapePathVisitor& a_Visitor);

	private:

		enum CommandType
		{
			eCommandType_Shape,
			eCommandType_Move,
			eCommandType_Line,
			eCommandType_CurveConic,
			eCommandType_CurveQuadratic
		};

		std::vector<CommandType> m_Commands;
		std::vector<glm::vec2> m_Positions;

	}; // class ShapePath

}; // namespace ExLibris

#endif