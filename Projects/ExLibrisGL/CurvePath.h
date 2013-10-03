#ifndef _CURVEPATH_H_
#define _CURVEPATH_H_

#include "CurveSettings.h"
#include "Polygon.h"

#include <glm/glm.hpp>
#include <vector>

namespace ExLibris
{

	class CurvePath
	{
	
	public:
	
		CurvePath();
		~CurvePath();

		size_t GetCommandCount() const;

		size_t GetPositionCount() const;
		const glm::vec2& GetPosition(size_t a_Index) const;

		void Move(const glm::vec2& a_Position);
		void LineTo(const glm::vec2& a_To);
		void ConicCurveTo(const glm::vec2& a_Control, const glm::vec2& a_To);
		void QuadraticCurveTo(const glm::vec2& a_ControlA, const glm::vec2& a_ControlB, const glm::vec2& a_To);

		std::vector<Polygon> BuildPolygons(const CurveSettings& a_Settings) const;

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