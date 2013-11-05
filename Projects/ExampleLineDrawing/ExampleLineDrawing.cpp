// STL

#include <iostream>
#include <map>
#include <string>
#include <vector>

// Framework

#include <Application.h>
#include <MeshOpenGL.h>
#include <ShaderProgram.h>

namespace fw = Framework;

// GLM

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OpenGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// ExLibris

#include <LineShape.h>
#include <Polygon.h>

// Main

class ExampleLineDrawing
	: public fw::Application
{

public:

	ExampleLineDrawing(int a_ArgumentCount, const char** a_Arguments)
		: fw::Application(a_ArgumentCount, a_Arguments)
		, m_ProgramLines(nullptr)
		, m_ProgramTriangles(nullptr)
		, m_MeshLines(nullptr)
		, m_OptionDrawLines(false)
	{
	}

	bool ParseCommandLine(int a_ArgumentCount, const char** a_Arguments)
	{
		return true;
	}

	bool Initialize()
	{
		_LoadShaders();

		m_MeshLines = new fw::MeshOpenGL();

		m_OptionLineQuality.quality = ExLibris::LineMeshOptions::eQuality_Fast;
		m_OptionLineQuality.thickness = 20.0f;

		return true;
	}

	void Update(float a_DeltaTime)
	{
	}

	void Render()
	{
		char window_title[256] = { 0 };
		sprintf(window_title, "ExLibris - (%i, %i)", (int)m_MousePosition.x, (int)m_MousePosition.y);
		glfwSetWindowTitle(GetWindow(), window_title);

		float ratio;
		int width, height;

		glfwGetFramebufferSize(GetWindow(), &width, &height);
		ratio = width / (float) height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4x4 projection = glm::ortho<float>(
			0.0f, (float)width,
			(float)height, 0.0f,
			-1.0f, 1.0f
		);

		glm::mat4x4 modelview;
		//modelview = glm::translate(modelview, g_CameraPosition);
		//modelview = glm::scale(modelview, glm::vec3(g_CameraZoom, g_CameraZoom, 1.0f));

		glm::mat4x4 mvp = projection * modelview;

		m_MousePath += m_MousePosition;

		ExLibris::LineShape shape;
		shape.AddPolygon(m_MousePath);

		ExLibris::MeshBuilder* builder = shape.BuildOutlineMesh(m_OptionLineQuality);

		m_MousePath.positions.pop_back();

		if (builder != nullptr && builder->GetVertexCount() > 0)
		{
			builder->Accept(*m_MeshLines);

			fw::ShaderProgram* outline_program = nullptr;
			glm::vec4 outline_color;

			if (m_OptionDrawLines)
			{
				outline_program = m_ProgramLines;
				outline_color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else
			{
				outline_program = m_ProgramTriangles;
				outline_color = glm::vec4(0.0f, 0.0f, 0.75f, 1.0f);
			}

			glUseProgram(outline_program->GetHandle());
			glUniformMatrix4fv(outline_program->FindUniform("matModelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));
			glUniform4fv(outline_program->FindUniform("uniColor"), 1, glm::value_ptr(outline_color));

			GLint attribute_position = outline_program->FindAttribute("attrPosition");
			glEnableVertexAttribArray(attribute_position);

			glBindBuffer(GL_ARRAY_BUFFER, m_MeshLines->GetBuffer());
			glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

			glDrawArrays(GL_TRIANGLES, 0, m_MeshLines->GetVertexCount());

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glUseProgram(0);
		}

		if (builder != nullptr)
		{
			delete builder;
		}
	}

	void Destroy()
	{
		delete m_MeshLines;

		if (m_ProgramLines != nullptr)
		{
			delete m_ProgramLines;
		}
		if (m_ProgramTriangles != nullptr)
		{
			delete m_ProgramTriangles;
		}
	}

private:

	void OnKeyReleased(int a_Key, int a_ScanCode, int a_Modifiers)
	{
		switch (a_Key)
		{

		case GLFW_KEY_ESCAPE:
			{
				Quit();

			} break;

		case GLFW_KEY_F5:
			{
				_LoadShaders();

			} break;

		case GLFW_KEY_UP:
			{
				m_OptionLineQuality.thickness += 1.0f;

			} break;

		case GLFW_KEY_DOWN:
			{
				m_OptionLineQuality.thickness -= 1.0f;

			} break;

		case GLFW_KEY_1:
			{
				m_OptionDrawLines = !m_OptionDrawLines;

			} break;

		case GLFW_KEY_2:
			{
				if (m_OptionLineQuality.quality == ExLibris::LineMeshOptions::eQuality_Fast)
				{
					m_OptionLineQuality.quality = ExLibris::LineMeshOptions::eQuality_Gapless;
				}
				else if (m_OptionLineQuality.quality == ExLibris::LineMeshOptions::eQuality_Gapless)
				{
					m_OptionLineQuality.quality = ExLibris::LineMeshOptions::eQuality_Fast;
				}

			} break;

		}
	}

	void OnMouseMoved(const glm::vec2& a_Position)
	{
		m_MousePosition = a_Position;
	}

	void OnButtonReleased(int a_Button, const glm::vec2& a_Position, int a_Modifiers)
	{
		if (a_Button == GLFW_MOUSE_BUTTON_1)
		{
			m_MousePath += a_Position;
		}
	}

private:

	void _LoadShaders()
	{
		try
		{
			if (m_ProgramTriangles == nullptr)
			{
				m_ProgramTriangles = new fw::ShaderProgram();
			}

			m_ProgramTriangles->LoadSourceFromFile(GL_VERTEX_SHADER, "Shaders/Triangles2D.vert");
			m_ProgramTriangles->LoadSourceFromFile(GL_FRAGMENT_SHADER, "Shaders/Triangles2D.frag");
			m_ProgramTriangles->Link();

			if (m_ProgramLines == nullptr)
			{
				m_ProgramLines = new fw::ShaderProgram();
			}

			m_ProgramLines->LoadSourceFromFile(GL_VERTEX_SHADER, "Shaders/Lines2D.vert");
			m_ProgramLines->LoadSourceFromFile(GL_FRAGMENT_SHADER, "Shaders/Lines2D.frag");
			m_ProgramLines->LoadSourceFromFile(GL_GEOMETRY_SHADER, "Shaders/Lines2D.geom");
			m_ProgramLines->Link();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

private:

	glm::vec2 m_MousePosition;
	ExLibris::Polygon m_MousePath;

	bool m_OptionDrawLines;
	ExLibris::LineMeshOptions m_OptionLineQuality;

	fw::ShaderProgram* m_ProgramTriangles;
	fw::ShaderProgram* m_ProgramLines;
	fw::MeshOpenGL* m_MeshLines;

}; // class ExampleLineDrawing

int main(int argc, const char** argv)
{
	ExampleLineDrawing application(argc, argv);
	return application.Run();
}
