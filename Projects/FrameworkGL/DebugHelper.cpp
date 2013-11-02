#include "FrameworkGL.PCH.h"

#include "DebugHelper.h"

// GLM

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ExLibris

#include <FontFace.h>
#include <IFont.h>
#include <TextLayout.h>

// Framework

#include "FontSystem.h"

namespace Framework
{

	static const std::string g_TextSourceVertex = "\
		#version 330 core\n \
		in vec2 attrPosition; \
		in vec2 attrTextureCoordinate0; \
		out vec2 vertTextureCoordinate; \
		uniform mat4 matModelViewProjection; \
		void main() \
		{ \
			gl_Position = matModelViewProjection * vec4(attrPosition, 0.0, 1.0); \
			vertTextureCoordinate = attrTextureCoordinate0; \
		}";

	static const std::string g_TextSourceFragment = "\
		#version 330 core\n \
		in vec2 vertTextureCoordinate; \
		uniform sampler2D texTexture0; \
		uniform vec2 uniTextureDimensions; \
		uniform vec4 uniTextColor; \
		out vec4 fragColor; \
		void main() \
		{ \
			vec4 color_sample = uniTextColor * texture(texTexture0, vertTextureCoordinate).a; \
			vec2 shadow_offset = vec2(-1.0, -1.0) / uniTextureDimensions; \
			vec4 shadow_sample = vec4(0.25, 0.25, 0.25, texture(texTexture0, vertTextureCoordinate + shadow_offset).a); \
			fragColor = mix(shadow_sample, color_sample, color_sample.a); \
		}";

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

	struct GlyphVertex
	{
		enum Offset
		{
			eOffset_Position = 0,
			eOffset_TextureCoordinate = sizeof(glm::vec2)
		};

		glm::vec2 position;
		glm::vec2 texture_coordinate;
	};

	DebugHelper::DebugHelper()
		: m_Font(nullptr)
		, m_FontFace(nullptr)
		, m_RenderStateText(nullptr)
		, m_RenderStateLines(nullptr)
	{
		m_Font = new FontSystem;

		ExLibris::FaceOptions options;
		m_FontFace = m_Font->CreateFace(options);

		_CreateTextState();
		_CreateLinesState();
	}
	
	DebugHelper::~DebugHelper()
	{
		Clear();

		delete m_FontFace;
		delete m_Font;

		glDeleteShader(m_RenderStateLines->vertex_shader);
		glDeleteShader(m_RenderStateLines->fragment_shader);
		glDeleteProgram(m_RenderStateLines->program);
		delete m_RenderStateLines;

		glDeleteShader(m_RenderStateText->vertex_shader);
		glDeleteShader(m_RenderStateText->fragment_shader);
		glDeleteProgram(m_RenderStateText->program);
		glDeleteBuffers(1, &m_RenderStateText->vertex_buffer);
		glDeleteBuffers(1, &m_RenderStateText->element_buffer);
		glDeleteVertexArrays(1, &m_RenderStateText->vertex_attribute_buffer);
		delete m_RenderStateText;
	}

	void DebugHelper::AddLine(const glm::vec2& a_Start, const glm::vec2& a_End, float a_Thickness /*= 1.0f*/, const glm::vec4& a_Color /*= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)*/)
	{
		RenderCommandLines* command = new RenderCommandLines(m_RenderStateLines);
		command->AddLine(a_Start, a_End);
		command->SetColor(a_Color);
		command->SetThickness(a_Thickness);
		command->Batch();

		m_Commands.push_back(command);
	}

	void DebugHelper::AddBox(const ExLibris::BoundingBox& a_Box, float a_Thickness /*= 1.0f*/, const glm::vec4& a_Color /*= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)*/)
	{
		glm::vec2 box_ul = a_Box.GetTopLeft();
		glm::vec2 box_ur = a_Box.GetTopRight();
		glm::vec2 box_ll = a_Box.GetBottomLeft();
		glm::vec2 box_lr = a_Box.GetBottomRight();

		RenderCommandLines* command = new RenderCommandLines(m_RenderStateLines);
		command->AddLine(box_ul, box_ur);
		command->AddLine(box_ur, box_lr);
		command->AddLine(box_lr, box_ll);
		command->AddLine(box_ll, box_ul);
		command->SetColor(a_Color);
		command->SetThickness(a_Thickness);
		command->Batch();

		m_Commands.push_back(command);
	}

	void DebugHelper::AddCircle(const glm::vec2& a_Position, float a_Radius, float a_Thickness /*= 1.0f*/, const glm::vec4& a_Color /*= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)*/)
	{
		RenderCommandLines* command = new RenderCommandLines(m_RenderStateLines);
		command->SetColor(a_Color);
		command->SetThickness(a_Thickness);

		int steps = 20;
		float degrees_step = 360.0f / (float)steps;
		float degrees = 0.0f;

		for (int i = 0; i < steps; ++i)
		{
			glm::vec2 position_current(
				a_Position.x + glm::cos(glm::radians(degrees)) * a_Radius,
				a_Position.y + glm::sin(glm::radians(degrees)) * a_Radius
			);

			glm::vec2 position_next(
				a_Position.x + glm::cos(glm::radians(degrees + degrees_step)) * a_Radius,
				a_Position.y + glm::sin(glm::radians(degrees + degrees_step)) * a_Radius
			);

			command->AddLine(position_current, position_next);

			degrees += degrees_step;
		}

		command->Batch();

		m_Commands.push_back(command);
	}
	
	void DebugHelper::AddText(const std::string& a_Text, const glm::vec2& a_Position, const glm::vec4& a_Color /*= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)*/)
	{
		RenderCommandText* command = new RenderCommandText(m_RenderStateText, m_FontFace);
		command->SetText(a_Text);
		command->SetPosition(a_Position);
		command->SetColor(a_Color);
		command->Batch();

		m_Commands.push_back(command);
	}

	void DebugHelper::Clear()
	{
		for (std::vector<IRenderCommand*>::const_iterator command_it = m_Commands.begin(); command_it != m_Commands.end(); ++command_it)
		{
			IRenderCommand* command = *command_it;

			delete command;
		}
		m_Commands.clear();
	}

	void DebugHelper::Render(int a_ScreenWidth, int a_ScreenHeight) const
	{
		if (m_Commands.size() == 0)
		{
			return;
		}

		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, a_ScreenWidth, a_ScreenHeight);

		glm::mat4x4 projection = glm::ortho<float>(
			0.0f, (float)a_ScreenWidth,
			(float)a_ScreenHeight, 0.0f,
			-1.0f, 1.0f
		);

		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (std::vector<IRenderCommand*>::const_iterator command_it = m_Commands.begin(); command_it != m_Commands.end(); ++command_it)
		{
			IRenderCommand* command = *command_it;

			command->Render(projection);
		}

		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);

		glPopAttrib();
	}

	void DebugHelper::_LoadShader(GLuint& a_Program, GLuint& a_VertexShader, const std::string& a_VertexSource, GLuint& a_FragmentShader, const std::string& a_FragmentSource)
	{
		GLint success;
		const GLchar* source_vertex = a_VertexSource.c_str();
		const GLchar* source_fragment = a_FragmentSource.c_str();

		a_VertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(a_VertexShader, 1, &source_vertex, nullptr);
		glCompileShader(a_VertexShader);

		glGetShaderiv(a_VertexShader, GL_COMPILE_STATUS, &success);
		if (success != GL_TRUE)
		{
			std::stringstream ss;
			ss << "Failed to compile vertex shader.";

			GLint info_log_length = 0;
			glGetShaderiv(a_VertexShader, GL_INFO_LOG_LENGTH, &info_log_length);
			if (info_log_length > 1)
			{
				GLchar* info_log = new GLchar[info_log_length + 1];
				glGetShaderInfoLog(a_VertexShader, info_log_length, 0, info_log);

				ss << std::endl << std::endl << info_log;

				delete [] info_log;
			}
			
			throw std::exception(ss.str().c_str());
		}

		a_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(a_FragmentShader, 1, &source_fragment, nullptr);
		glCompileShader(a_FragmentShader);

		glGetShaderiv(a_FragmentShader, GL_COMPILE_STATUS, &success);
		if (success != GL_TRUE)
		{
			std::stringstream ss;
			ss << "Failed to compile fragment shader.";

			GLint info_log_length = 0;
			glGetShaderiv(a_FragmentShader, GL_INFO_LOG_LENGTH, &info_log_length);
			if (info_log_length > 1)
			{
				GLchar* info_log = new GLchar[info_log_length + 1];
				glGetShaderInfoLog(a_FragmentShader, info_log_length, 0, info_log);

				ss << std::endl << std::endl << info_log;

				delete [] info_log;
			}

			throw std::exception(ss.str().c_str());
		}

		a_Program = glCreateProgram();
		glAttachShader(a_Program, a_VertexShader);
		glAttachShader(a_Program, a_FragmentShader);
		glLinkProgram(a_Program);

		GLchar* log_program = nullptr;

		GLint success_link = 0;
		glGetProgramiv(a_Program, GL_LINK_STATUS, &success_link);

		GLint success_validate = 0;
		glValidateProgram(a_Program);
		glGetProgramiv(a_Program, GL_VALIDATE_STATUS, &success_validate);

		if (success_link != GL_TRUE || success_validate != GL_TRUE)
		{
			std::stringstream ss;

			if (success_link != GL_TRUE)
			{
				ss << "Linking program failed.";
			}
			else if (success_validate != GL_TRUE)
			{
				ss << "Program validation failed.";
			}

			GLint info_log_length = 0;
			glGetShaderiv(a_Program, GL_INFO_LOG_LENGTH, &info_log_length);
			if (info_log_length > 1)
			{
				GLchar* info_log = new GLchar[info_log_length + 1];
				glGetShaderInfoLog(a_Program, info_log_length, 0, info_log);

				ss << std::endl << std::endl << info_log;

				delete [] info_log;
			}

			throw std::exception(ss.str().c_str());
		}
	}

	void DebugHelper::_CreateTextState()
	{
		m_RenderStateText = new RenderCommandText::RenderState;

		_LoadShader(
			m_RenderStateText->program,
			m_RenderStateText->vertex_shader, g_TextSourceVertex,
			m_RenderStateText->fragment_shader, g_TextSourceFragment
		);

		GLint attribute_position = glGetAttribLocation(m_RenderStateText->program, "attrPosition");
		GLint attribute_texturecoordinate0 = glGetAttribLocation(m_RenderStateText->program, "attrTextureCoordinate0");
		if (attribute_position == -1 || attribute_texturecoordinate0 == -1)
		{
			throw std::exception("Failed to find attribute locations.");
		}

		m_RenderStateText->uniform_modelviewprojection = glGetUniformLocation(m_RenderStateText->program, "matModelViewProjection");
		m_RenderStateText->uniform_texture0 = glGetUniformLocation(m_RenderStateText->program, "texTexture0");
		m_RenderStateText->uniform_texturedimensions = glGetUniformLocation(m_RenderStateText->program, "uniTextureDimensions");
		m_RenderStateText->uniform_textcolor = glGetUniformLocation(m_RenderStateText->program, "uniTextColor");
		if (m_RenderStateText->uniform_modelviewprojection == -1 || m_RenderStateText->uniform_texture0 == -1 || m_RenderStateText->uniform_texturedimensions == -1 || m_RenderStateText->uniform_textcolor == -1)
		{
			throw new std::exception("Failed to find uniform locations.");
		}

		// vertex buffer

		glGenBuffers(1, &m_RenderStateText->vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderStateText->vertex_buffer);

			GlyphVertex vertex_data[4] = {
				{
					glm::vec2(0.0f, 0.0f),
					glm::vec2(0.0f, 0.0f)
				},
				{
					glm::vec2(1.0f, 0.0f),
					glm::vec2(1.0f, 0.0f)
				},
				{
					glm::vec2(0.0f, 1.0f),
					glm::vec2(0.0f, 1.0f)
				},
				{
					glm::vec2(1.0f, 1.0f),
					glm::vec2(1.0f, 1.0f)
				},
			};

			glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GlyphVertex), vertex_data, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// element buffer

		glGenBuffers(1, &m_RenderStateText->element_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderStateText->element_buffer);

			GLuint element_data[6] = {
				1, 0, 2,
				1, 2, 3
			};

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), element_data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// vertex attribute buffer

		glGenVertexArrays(1, &m_RenderStateText->vertex_attribute_buffer);
		glBindVertexArray(m_RenderStateText->vertex_attribute_buffer);

			glBindBuffer(GL_ARRAY_BUFFER, m_RenderStateText->vertex_buffer);

			glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (const GLvoid*)GlyphVertex::eOffset_Position);
			glEnableVertexAttribArray(attribute_position);

			glVertexAttribPointer(attribute_texturecoordinate0, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (const GLvoid*)GlyphVertex::eOffset_TextureCoordinate);
			glEnableVertexAttribArray(attribute_texturecoordinate0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderStateText->element_buffer);

		glBindVertexArray(0);
	}

	void DebugHelper::_CreateLinesState()
	{
		m_RenderStateLines = new RenderCommandLines::RenderState;

		_LoadShader(
			m_RenderStateLines->program,
			m_RenderStateLines->vertex_shader, g_LinesSourceVertex,
			m_RenderStateLines->fragment_shader, g_LinesSourceFragment
		);

		m_RenderStateLines->attribute_position = glGetAttribLocation(m_RenderStateLines->program, "attrPosition");
		if (m_RenderStateLines->attribute_position == -1)
		{
			throw std::exception("Failed to find attribute locations.");
		}

		m_RenderStateLines->uniform_modelviewprojection = glGetUniformLocation(m_RenderStateLines->program, "matModelViewProjection");
		m_RenderStateLines->uniform_color = glGetUniformLocation(m_RenderStateLines->program, "uniColor");
		if (m_RenderStateLines->uniform_modelviewprojection == -1 || m_RenderStateLines->uniform_color == -1)
		{
			throw new std::exception("Failed to find uniform locations.");
		}
	}

}; // namespace Framework