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

		void Move(const glm::vec2& a_Position);
		void LineTo(const glm::vec2& a_To);
		void ConicCurveTo(const glm::vec2& a_Control, const glm::vec2& a_To);
		void QuadraticCurveTo(const glm::vec2& a_ControlA, const glm::vec2& a_ControlB, const glm::vec2& a_To);

		std::vector<Polygon> BuildPolygons(const CurveSettings& a_Settings);

		std::vector<Polygon> ConvertToPolygons(const CurveSettings& a_Settings) const;

	private:

		enum ShapeResult
		{
			eShapeResult_Valid,
			eShapeResult_Next,
			eShapeResult_End
		};

		ShapeResult _NextCurvePosition(glm::vec2& a_Target, const CurveSettings& a_Settings);

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

		std::vector<glm::vec2>::iterator m_CommandPositionCurrent;
		std::vector<CommandType>::iterator m_CommandCurrent;

		glm::vec2 m_CurvePositionPrevious;
		glm::vec2 m_CurveFrom;
		glm::vec2 m_CurveControlA;
		glm::vec2 m_CurveControlB;
		glm::vec2 m_CurveTo;
		bool m_CurveShapeNext;
		bool m_CurveStarted;
		int m_CurveStep;
		float m_CurveTimeDelta;

	}; // class CurvePath

}; // namespace ExLibris

#endif