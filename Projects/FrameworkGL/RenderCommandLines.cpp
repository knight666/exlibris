#include "FrameworkGL.PCH.h"

#include "RenderCommandLines.h"

// GLM

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Framwork

#include "ShaderProgram.h"

namespace Framework
{
	
	static const std::string g_LinesSourceVertex = "\
		#version 330 core\n \
		in vec2 attrPosition; \
		uniform mat4 matModelViewProjection; \
		void main() \
		{ \
			gl_Position = matModelViewProjection * vec4(attrPosition, 0.0, 1.0); \
		}";

	static const std::string g_LinesSourceFragment = "\
		#version 330 core\n\
		uniform vec4 uniColor; \
		out vec4 fragColor; \
		void main() \
		{ \
			fragColor = uniColor; \
		}";

	RenderCommandLines::RenderState* RenderCommandLines::CreateRenderState()
	{
		RenderState* state = new RenderState;

		state->program = ShaderProgram::Create(g_LinesSourceVertex, g_LinesSourceFragment);

		state->attribute_position = state->program->FindAttribute("attrPosition");
		if (state->attribute_position == -1)
		{
			throw std::exception("Failed to find attribute locations.");
		}

		state->uniform_modelviewprojection = state->program->FindUniform("matModelViewProjection");
		state->uniform_color = state->program->FindUniform("uniColor");
		if (state->uniform_modelviewprojection == -1 || state->uniform_color == -1)
		{
			throw new std::exception("Failed to find uniform locations.");
		}

		return state;
	}

	RenderCommandLines::RenderCommandLines(RenderState* a_State)
		: m_State(a_State)
		, m_Color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		, m_Thickness(1.0f)
		, m_VertexBuffer(0)
		, m_ElementBuffer(0)
		, m_VertexArrayBuffer(0)
	{
	}
	
	RenderCommandLines::~RenderCommandLines()
	{
		if (m_VertexBuffer != 0)
		{
			glDeleteBuffers(1, &m_VertexBuffer);
		}
		if (m_ElementBuffer != 0)
		{
			glDeleteBuffers(1, &m_ElementBuffer);
		}
		if (m_VertexArrayBuffer != 0)
		{
			glDeleteVertexArrays(1, &m_VertexArrayBuffer);
		}
	}

	void RenderCommandLines::SetColor(const glm::vec4& a_Color)
	{
		m_Color = a_Color;
	}

	void RenderCommandLines::SetThickness(float a_Thickness)
	{
		m_Thickness = a_Thickness;
	}

	void RenderCommandLines::AddLine(const glm::vec2& a_Start, const glm::vec2& a_End)
	{
		m_Lines.push_back(ExLibris::Line(a_Start, a_End));
	}

	void RenderCommandLines::Batch()
	{
		glm::vec2* position_data = new glm::vec2[m_Lines.size() * 4];
		GLuint* element_data = new GLuint[m_Lines.size() * 6];

		glm::vec2* dst_position = position_data;
		GLuint* dst_element = element_data;
		GLuint index = 0;

		for (std::vector<ExLibris::Line>::const_iterator line_it = m_Lines.begin(); line_it != m_Lines.end(); ++line_it)
		{
			const ExLibris::Line& line = *line_it;

			ExLibris::Quad quad = line.ConstructQuad(m_Thickness);

			*dst_position++ = quad.ul;
			*dst_position++ = quad.ur;
			*dst_position++ = quad.ll;
			*dst_position++ = quad.lr;

			*dst_element++ = index + 1;
			*dst_element++ = index + 0;
			*dst_element++ = index + 2;

			*dst_element++ = index + 1;
			*dst_element++ = index + 2;
			*dst_element++ = index + 3;

			index += 4;
		}

		glGenVertexArrays(1, &m_VertexArrayBuffer);
		glBindVertexArray(m_VertexArrayBuffer);

			glGenBuffers(1, &m_VertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, m_Lines.size() * 4 * sizeof(glm::vec2), position_data, GL_STATIC_DRAW);
				delete [] position_data;

			glVertexAttribPointer(m_State->attribute_position, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
			glEnableVertexAttribArray(m_State->attribute_position);

			glGenBuffers(1, &m_ElementBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Lines.size() * 6 * sizeof(GLuint), element_data, GL_STATIC_DRAW);
				delete [] element_data;

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void RenderCommandLines::Render(const glm::mat4x4& a_Projection) const
	{
		glUseProgram(m_State->program->GetHandle());

		glUniform4fv(m_State->uniform_color, 1, glm::value_ptr(m_Color));

		glUniformMatrix4fv(m_State->uniform_modelviewprojection, 1, GL_FALSE, glm::value_ptr(a_Projection));

		glBindVertexArray(m_VertexArrayBuffer);

		glDrawElements(GL_TRIANGLES, m_Lines.size() * 6, GL_UNSIGNED_INT, 0);
	}

}; // namespace Framework