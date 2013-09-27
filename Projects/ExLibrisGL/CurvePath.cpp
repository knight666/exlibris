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

	bool CurvePath::StartCursor(const CurveSettings& a_Settings)
	{
		if (m_Commands.size() == 0)
		{
			return false;
		}

		m_Cursor.settings = a_Settings;
		m_Cursor.curve_previous = glm::vec2();
		m_Cursor.curve_started = false;
		m_Cursor.curve_step = 0;
		m_Cursor.curve_time_delta = 0.0f;
		m_Cursor.position_it = m_Positions.begin();
		m_Cursor.command_it = m_Commands.begin();

		return true;
	}

	glm::vec2 CurvePath::ReadCursorPosition()
	{
		glm::vec2 position;

		if (m_Cursor.command_it == m_Commands.end())
		{
			return position;
		}

		switch (*m_Cursor.command_it)
		{

		case eCommandType_Move:
			{
				position = *m_Cursor.position_it++;
				++m_Cursor.command_it;

			} break;

		case eCommandType_Line:
			{
				position = *m_Cursor.position_it++;
				++m_Cursor.command_it;

			} break;

		case eCommandType_CurveConic:
			{
				if (m_Cursor.curve_started && m_Cursor.curve_step >= m_Cursor.settings.precision)
				{
					position = m_Cursor.curve_to;

					m_Cursor.curve_started = false;
					m_Cursor.curve_step = 0;

					++m_Cursor.command_it;

					break;
				}

				if (!m_Cursor.curve_started)
				{
					m_Cursor.curve_from = m_Cursor.curve_previous;
					m_Cursor.curve_control_a = *m_Cursor.position_it++;
					m_Cursor.curve_to = *m_Cursor.position_it++;

					if (m_Cursor.settings.precision <= 1)
					{
						position = m_Cursor.curve_to;

						++m_Cursor.command_it;

						break;
					}

					m_Cursor.curve_step = 1;
					m_Cursor.curve_time_delta = 1.0f / (float)m_Cursor.settings.precision;

					m_Cursor.curve_started = true;
				}

				float time = m_Cursor.curve_step * m_Cursor.curve_time_delta;

				glm::vec2 ac = glm::mix(m_Cursor.curve_from, m_Cursor.curve_control_a, time);
				glm::vec2 cb = glm::mix(m_Cursor.curve_control_a, m_Cursor.curve_to, time);
				position = glm::mix(ac, cb, time);

				m_Cursor.curve_step++;

			} break;

		case eCommandType_CurveQuadratic:
			{
				if (m_Cursor.curve_started && m_Cursor.curve_step >= m_Cursor.settings.precision)
				{
					position = m_Cursor.curve_to;

					m_Cursor.curve_started = false;
					m_Cursor.curve_step = 0;

					++m_Cursor.command_it;

					break;
				}

				if (!m_Cursor.curve_started)
				{
					m_Cursor.curve_from = m_Cursor.curve_previous;
					m_Cursor.curve_control_a = *m_Cursor.position_it++;
					m_Cursor.curve_control_b = *m_Cursor.position_it++;
					m_Cursor.curve_to = *m_Cursor.position_it++;

					if (m_Cursor.settings.precision <= 1)
					{
						position = m_Cursor.curve_to;

						++m_Cursor.command_it;

						break;
					}

					m_Cursor.curve_step = 1;
					m_Cursor.curve_time_delta = 1.0f / (float)m_Cursor.settings.precision;

					m_Cursor.curve_started = true;
				}

				float time = m_Cursor.curve_step * m_Cursor.curve_time_delta;

				glm::vec2 ab = glm::mix(m_Cursor.curve_from, m_Cursor.curve_control_a, time);
				glm::vec2 bc = glm::mix(m_Cursor.curve_control_a, m_Cursor.curve_control_b, time);
				glm::vec2 cd = glm::mix(m_Cursor.curve_control_b, m_Cursor.curve_to, time);

				glm::vec2 mixed_a = glm::mix(ab, bc, time);
				glm::vec2 mixed_b = glm::mix(bc, cd, time);

				position = glm::mix(mixed_a, mixed_b, time);

				m_Cursor.curve_step++;

			} break;

		}

		m_Cursor.curve_previous = position;
		
		return position;
	}

	bool CurvePath::IsCursorNextShape() const
	{
		return ((m_Cursor.command_it != m_Commands.begin()) && (*m_Cursor.command_it == eCommandType_Move));
	}

	bool CurvePath::IsCursorValid() const
	{
		return ((m_Commands.size() > 0) && (m_Cursor.command_it != m_Commands.end()));
	}

	std::vector<Polygon> CurvePath::BuildPolygons(const CurveSettings& a_Settings)
	{
		std::vector<Polygon> shapes;

		if (!StartCursor(a_Settings))
		{
			return shapes;
		}

		Polygon shape;
		shapes.push_back(shape);

		Polygon* shape_current = &shapes.back();

		while (IsCursorValid())
		{
			if (IsCursorNextShape())
			{
				Polygon shape;
				shapes.push_back(shape);

				shape_current = &shapes.back();
			}

			*shape_current += ReadCursorPosition();
		}

		return shapes;
	}

}; // namespace ExLibris