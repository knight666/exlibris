#include "ExLibrisGL.PCH.h"

#include "ShapePath.h"

namespace ExLibris
{

	ShapePath::ShapePath()
	{
	}
	
	ShapePath::~ShapePath()
	{
	}

	void ShapePath::Move(const glm::vec2& a_Position)
	{
		m_Commands.push_back(eCommandType_Move);
		m_Positions.push_back(a_Position);
	}

	void ShapePath::LineTo(const glm::vec2& a_To)
	{
		m_Commands.push_back(eCommandType_Line);
		m_Positions.push_back(a_To);
	}

	void ShapePath::Accept(IShapePathVisitor& a_Visitor)
	{
		std::vector<glm::vec2>::iterator position_it = m_Positions.begin();

		for (std::vector<CommandType>::iterator command_it = m_Commands.begin(); command_it != m_Commands.end(); ++command_it)
		{
			switch (*command_it)
			{

			case eCommandType_Move:
				{
					a_Visitor.VisitShapeStart();
					a_Visitor.VisitShapePosition(*position_it++);

				} break;

			case eCommandType_Line:
				{
					a_Visitor.VisitShapePosition(*position_it++);

				} break;

			}
		}

		a_Visitor.VisitShapeEnd();
	}

}; // namespace ExLibris