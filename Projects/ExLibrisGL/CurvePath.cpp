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

	void CurvePath::Accept(ICurvePathVisitor& a_Visitor, const CurveSettings& a_Settings)
	{
		std::vector<glm::vec2>::iterator position_it = m_Positions.begin();

		for (std::vector<CommandType>::iterator command_it = m_Commands.begin(); command_it != m_Commands.end(); ++command_it)
		{
			switch (*command_it)
			{

			case eCommandType_Move:
				{
					a_Visitor.VisitCurveStart();
					a_Visitor.VisitCurvePosition(*position_it++);

				} break;

			case eCommandType_Line:
				{
					a_Visitor.VisitCurvePosition(*position_it++);

				} break;

			}
		}

		a_Visitor.VisitCurveEnd();
	}

}; // namespace ExLibris