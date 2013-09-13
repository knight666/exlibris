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

	void CurvePath::QuadraticCurveTo(const glm::vec2& a_ControlA, const glm::vec2& a_ControlB, const glm::vec2& a_To)
	{
		m_Commands.push_back(eCommandType_CurveQuadratic);
		m_Positions.push_back(a_ControlA);
		m_Positions.push_back(a_ControlB);
		m_Positions.push_back(a_To);
	}

	std::vector<Shape> CurvePath::ConvertToShapes(const CurveSettings& a_Settings) const
	{
		std::vector<Shape> shapes;

		if (m_Commands.size() == 0)
		{
			return shapes;
		}

		Shape shape;
		shapes.push_back(shape);

		std::vector<glm::vec2>::const_iterator position_it = m_Positions.begin();
		glm::vec2 position_previous;

		for (std::vector<CommandType>::const_iterator command_it = m_Commands.begin(); command_it != m_Commands.end(); ++command_it)
		{
			Shape& shape_current = shapes.back();

			switch (*command_it)
			{

			case eCommandType_Move:
				{
					// check if it's actually a new shape and not the first move command

					if (command_it != m_Commands.begin())
					{
						Shape shape;
						shapes.push_back(shape);
					}

					shape_current.positions.push_back(*position_it);

				} break;

			case eCommandType_Line:
				{
					shape_current.positions.push_back(*position_it);

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

							shape_current.positions.push_back(mixed);

							time += delta;
						}
					}

					shape_current.positions.push_back(to);

				} break;

			case eCommandType_CurveQuadratic:
				{
					glm::vec2 from = position_previous;
					glm::vec2 control_a = *position_it++;
					glm::vec2 control_b = *position_it++;
					glm::vec2 to = *position_it;

					if (a_Settings.precision > 1)
					{
						float delta = 1.0f / (float)a_Settings.precision;
						float time = delta;

						for (int step = 1; step < a_Settings.precision; ++step)
						{
							glm::vec2 ab = glm::mix(from, control_a, time);
							glm::vec2 bc = glm::mix(control_a, control_b, time);
							glm::vec2 cd = glm::mix(control_b, to, time);

							glm::vec2 mixed_a = glm::mix(ab, bc, time);
							glm::vec2 mixed_b = glm::mix(bc, cd, time);

							glm::vec2 mixed = glm::mix(mixed_a, mixed_b, time);

							shape_current.positions.push_back(mixed);

							time += delta;
						}
					}

					shape_current.positions.push_back(to);

				} break;

			}

			position_previous = *position_it++;
		}

		return shapes;
	}

}; // namespace ExLibris