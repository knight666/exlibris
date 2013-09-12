#ifndef _CURVEPATH_H_
#define _CURVEPATH_H_

#include "CurveSettings.h"
#include "ICurvePathVisitor.h"

#include <glm/glm.hpp>
#include <vector>

namespace ExLibris
{

	class CurvePath
	{
	
	public:
	
		CurvePath();
		~CurvePath();

		void Move(const glm::vec2& a_Position);
		void LineTo(const glm::vec2& a_To);
		void ConicCurveTo(const glm::vec2& a_Control, const glm::vec2& a_To);
		void QuadraticCurveTo(const glm::vec2& a_ControlA, const glm::vec2& a_ControLB, const glm::vec2& a_To);

		void Accept(ICurvePathVisitor& a_Visitor, const CurveSettings& a_Settings);

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

	}; // class CurvePath

}; // namespace ExLibris

#endif