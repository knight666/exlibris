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

		bool StartCursor(const CurveSettings& a_Settings);
		glm::vec2 ReadCursorPosition();
		bool IsCursorNextShape() const;
		bool IsCursorValid() const;

		std::vector<Polygon> BuildPolygons(const CurveSettings& a_Settings);

		std::vector<Polygon> ConvertToPolygons(const CurveSettings& a_Settings) const;

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

		struct CursorState
		{
			CurveSettings settings;
			std::vector<CommandType>::iterator command_it;
			std::vector<glm::vec2>::iterator position_it;
			glm::vec2 curve_previous;
			glm::vec2 curve_from;
			glm::vec2 curve_control_a;
			glm::vec2 curve_control_b;
			glm::vec2 curve_to;
			bool curve_started;
			int curve_step;
			float curve_time_delta;
		};
		CursorState m_Cursor;

	}; // class CurvePath

}; // namespace ExLibris

#endif