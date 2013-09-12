#include "ExLibrisGL.PCH.h"

#include "CurvePath.h"

namespace ExLibris
{

	CurvePath::CurvePath()
	{
	}
	
	CurvePath::~CurvePath()
	{
	}

	void CurvePath::Move(const glm::vec2& a_Position)
	{
		m_Commands.push_back(eCommandType_Move);
		m_Positions.push_back(a_Position);
	}

	void CurvePath::LineTo(const glm::vec2& a_To)
	{
		m_Commands.push_back(eCommandType_Line);
		m_Positions.push_back(a_To);
	}

	void CurvePath::ConicCurveTo(const glm::vec2& a_Control, const glm::vec2& a_To)
	{
		m_Commands.push_back(eCommandType_CurveConic);
		m_Positions.push_back(a_Control);
		m_Positions.push_back(a_To);
	}

	void CurvePath::Accept(ICurvePathVisitor& a_Visitor, const CurveSettings& a_Settings)
	{
		std::vector<glm::vec2>::iterator position_it = m_Positions.begin();
		glm::vec2 position_previous;
		int shape_count = 0;

		for (std::vector<CommandType>::iterator command_it = m_Commands.begin(); command_it != m_Commands.end(); ++command_it)
		{
			if (*command_it != eCommandType_Move && shape_count == 0)
			{
				a_Visitor.VisitCurveStart();
			}

			switch (*command_it)
			{

			case eCommandType_Move:
				{
					if (shape_count > 0)
					{
						a_Visitor.VisitCurveEnd();
					}

					a_Visitor.VisitCurveStart();
					shape_count++;

					a_Visitor.VisitCurvePosition(*position_it);

				} break;

			case eCommandType_Line:
				{
					a_Visitor.VisitCurvePosition(*position_it);

				} break;

			case eCommandType_CurveConic:
				{
					glm::vec2 from = position_previous;
					glm::vec2 control = *position_it++;
					glm::vec2 to = *position_it;

					if (a_Settings.precision > 1)
					{
						float delta = 1.0f / (float)a_Settings.precision;
						float time = delta;

						for (int step = 1; step < a_Settings.precision; ++step)
						{
							glm::vec2 ac = glm::mix(from, control, time);
							glm::vec2 cb = glm::mix(control, to, time);
							glm::vec2 mixed = glm::mix(ac, cb, time);

							a_Visitor.VisitCurvePosition(mixed);

							time += delta;
						}
					}
					
					a_Visitor.VisitCurvePosition(to);

				} break;

			}

			position_previous = *position_it++;
		}

		a_Visitor.VisitCurveEnd();
	}

}; // namespace ExLibris