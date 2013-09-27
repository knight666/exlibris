#include "ExLibrisGL.PCH.h"

#include "CurvePath.h"

namespace ExLibris
{

	CurvePath::CurvePath()
		: m_CurveStarted(false)
		, m_CurveStep(0)
		, m_CurveTimeDelta(0.0f)
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

	std::vector<Polygon> CurvePath::ConvertToPolygons(const CurveSettings& a_Settings) const
	{
		std::vector<Polygon> shapes;

		if (m_Commands.size() == 0)
		{
			return shapes;
		}

		Polygon shape;
		shapes.push_back(shape);

		std::vector<glm::vec2>::const_iterator position_it = m_Positions.begin();
		glm::vec2 position_previous;

		for (std::vector<CommandType>::const_iterator command_it = m_Commands.begin(); command_it != m_Commands.end(); ++command_it)
		{
			Polygon* shape_current = &shapes.back();

			switch (*command_it)
			{

			case eCommandType_Move:
				{
					// check if it's actually a new shape and not the first move command

					if (command_it != m_Commands.begin())
					{
						Polygon shape;
						shapes.push_back(shape);

						shape_current = &shapes.back();
					}

					shape_current->positions.push_back(*position_it);

				} break;

			case eCommandType_Line:
				{
					shape_current->positions.push_back(*position_it);

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

							shape_current->positions.push_back(mixed);

							time += delta;
						}
					}

					shape_current->positions.push_back(to);

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

							shape_current->positions.push_back(mixed);

							time += delta;
						}
					}

					shape_current->positions.push_back(to);

				} break;

			}

			position_previous = *position_it++;
		}

		return shapes;
	}

	bool CurvePath::_NextShape()
	{
		bool is_move = (*m_CommandCurrent == eCommandType_Move);
		bool is_first_command = (m_CommandCurrent == m_Commands.begin());

		return is_move && !is_first_command;
	}

	CurvePath::ShapeResult CurvePath::_NextCurvePosition(glm::vec2& a_Target, const CurveSettings& a_Settings)
	{
		m_CurveShapeNext = false;
		bool next_command = false;

		switch (*m_CommandCurrent)
		{

		case eCommandType_Move:
			{
				a_Target = *m_CommandPositionCurrent++;
				m_CurveShapeNext = true;
				next_command = true;

			} break;

		case eCommandType_Line:
			{
				a_Target = *m_CommandPositionCurrent++;
				next_command = true;

			} break;

		case eCommandType_CurveConic:
			{
				if (m_CurveStarted && m_CurveStep >= a_Settings.precision)
				{
					a_Target = m_CurveTo;

					m_CurveStarted = false;
					m_CurveStep = 0;

					next_command = true;

					break;
				}

				if (!m_CurveStarted)
				{
					m_CurveFrom = m_CurvePositionPrevious;
					m_CurveControlA = *m_CommandPositionCurrent++;
					m_CurveTo = *m_CommandPositionCurrent++;

					if (a_Settings.precision <= 1)
					{
						a_Target = m_CurveTo;

						next_command = true;

						break;
					}

					m_CurveStep = 1;
					m_CurveTimeDelta = 1.0f / (float)a_Settings.precision;

					m_CurveStarted = true;
				}
				
				float time = m_CurveStep * m_CurveTimeDelta;

				glm::vec2 ac = glm::mix(m_CurveFrom, m_CurveControlA, time);
				glm::vec2 cb = glm::mix(m_CurveControlA, m_CurveTo, time);
				a_Target = glm::mix(ac, cb, time);

				m_CurveStep++;

			} break;

		case eCommandType_CurveQuadratic:
			{
				if (m_CurveStarted && m_CurveStep >= a_Settings.precision)
				{
					a_Target = m_CurveTo;

					m_CurveStarted = false;
					m_CurveStep = 0;

					next_command = true;

					break;
				}

				if (!m_CurveStarted)
				{
					m_CurveFrom = m_CurvePositionPrevious;
					m_CurveControlA = *m_CommandPositionCurrent++;
					m_CurveControlB = *m_CommandPositionCurrent++;
					m_CurveTo = *m_CommandPositionCurrent++;

					if (a_Settings.precision <= 1)
					{
						a_Target = m_CurveTo;

						next_command = true;

						break;
					}

					m_CurveStep = 1;
					m_CurveTimeDelta = 1.0f / (float)a_Settings.precision;

					m_CurveStarted = true;
				}

				float time = m_CurveStep * m_CurveTimeDelta;

				glm::vec2 ab = glm::mix(m_CurveFrom, m_CurveControlA, time);
				glm::vec2 bc = glm::mix(m_CurveControlA, m_CurveControlB, time);
				glm::vec2 cd = glm::mix(m_CurveControlB, m_CurveTo, time);

				glm::vec2 mixed_a = glm::mix(ab, bc, time);
				glm::vec2 mixed_b = glm::mix(bc, cd, time);

				a_Target = glm::mix(mixed_a, mixed_b, time);

				m_CurveStep++;

			} break;

		}

		m_CurvePositionPrevious = a_Target;

		if (next_command && ++m_CommandCurrent == m_Commands.end())
		{
			return eShapeResult_End;
		}
		else
		{
			return eShapeResult_Valid;
		}
	}

	std::vector<Polygon> CurvePath::BuildPolygons(const CurveSettings& a_Settings)
	{
		std::vector<Polygon> shapes;

		if (m_Commands.size() == 0)
		{
			return shapes;
		}

		Polygon shape;
		shapes.push_back(shape);

		Polygon* shape_current = &shapes.back();

		m_CommandPositionCurrent = m_Positions.begin();
		m_CommandCurrent = m_Commands.begin();

		glm::vec2 position;
		ShapeResult result = eShapeResult_Next;

		while (result != eShapeResult_End)
		{
			// check if it's actually a new shape and not the first move command

			if (m_CommandCurrent != m_Commands.begin() && *m_CommandCurrent == eCommandType_Move)
			{
				Polygon shape;
				shapes.push_back(shape);

				shape_current = &shapes.back();
			}

			result = _NextCurvePosition(position, a_Settings);

			*shape_current += position;
		}

		return shapes;
	}

}; // namespace ExLibris