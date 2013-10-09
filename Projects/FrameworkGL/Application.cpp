#include "FrameworkGL.PCH.h"

#include <iostream>
#include <windows.h>
#include <GL/glew.h>

#include "Application.h"

namespace Framework
{

	static void ProcessGLFWError(int a_ErrorCode, const char* a_Description)
	{
		std::cerr << a_Description << std::endl;
	}

	static void ProcessCharacter(GLFWwindow* a_Window, unsigned int a_Character)
	{
		Application* application = (Application*)glfwGetWindowUserPointer(a_Window);

		application->OnCharacter(a_Character);
	}

	static void ProcessKey(GLFWwindow* a_Window, int a_Key, int a_ScanCode, int a_Action, int a_Modifiers)
	{
		Application* application = (Application*)glfwGetWindowUserPointer(a_Window);

		if (a_Action == GLFW_PRESS)
		{
			application->OnKeyPressed(a_Key, a_ScanCode, a_Modifiers);
		}
		else if (a_Action == GLFW_RELEASE)
		{
			application->OnKeyReleased(a_Key, a_ScanCode, a_Modifiers);
		}
	}

	void ProcessButton(GLFWwindow* a_Window, int a_Button, int a_Action, int a_Modifiers)
	{
		Application* application = (Application*)glfwGetWindowUserPointer(a_Window);

		double cursor_position[2];
		glfwGetCursorPos(a_Window, &cursor_position[0], &cursor_position[1]);

		glm::vec2 position((float)cursor_position[0], (float)cursor_position[1]);

		if (a_Action == GLFW_PRESS)
		{
			application->OnButtonPressed(a_Button, position, a_Modifiers);
		}
		else if (a_Action == GLFW_RELEASE)
		{
			application->OnButtonReleased(a_Button, position, a_Modifiers);
		}
	}

	void ProcessCursor(GLFWwindow* a_Window, double a_X, double a_Y)
	{
		Application* application = (Application*)glfwGetWindowUserPointer(a_Window);

		glm::vec2 position((float)a_X, (float)a_Y);

		application->OnMouseMoved(position);
	}

	Application::Application(int a_ArgumentCount, const char** a_Arguments)
		: m_ArgumentCount(a_ArgumentCount)
		, m_Arguments(a_Arguments)
		, m_Running(false)
	{
	}
	
	Application::~Application()
	{
	}

	GLFWwindow* Application::GetWindow() const
	{
		return m_Window;
	}

	bool Application::IsRunning() const
	{
		return m_Running;
	}

	void Application::Quit()
	{
		m_Running = false;
	}

	bool Application::_InitializeOpenGL()
	{
		glfwSetErrorCallback(ProcessGLFWError);

		if (glfwInit() == 0)
		{
			std::cerr << "Failed to initialize GLFW." << std::endl;

			return false;
		}

		m_Window = glfwCreateWindow(640, 480, "ExLibris", nullptr, nullptr);
		if (m_Window == nullptr)
		{
			std::cerr << "Failed to create GLFW window." << std::endl;

			return false;
		}

		glfwMakeContextCurrent(m_Window);

		glfwSetCharCallback(m_Window, ProcessCharacter);
		glfwSetKeyCallback(m_Window, ProcessKey);
		glfwSetMouseButtonCallback(m_Window, ProcessButton);
		glfwSetCursorPosCallback(m_Window, ProcessCursor);

		glfwSetWindowUserPointer(m_Window, this);

		if (glewInit() != GLEW_OK)
		{
			std::cerr << "Failed to initialize GLEW." << std::endl;

			return false;
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_TEXTURE_2D);

		return true;
	}

	void Application::_DestroyOpenGL()
	{
		glfwTerminate();
	}

	int Application::Run()
	{
		ParseCommandLine(m_ArgumentCount, m_Arguments);

		if (!_InitializeOpenGL())
		{
			_DestroyOpenGL();

			return -1;
		}

		if (!Initialize())
		{
			return -1;
		}

		m_Running = true;

		unsigned long time_start = GetTickCount();
		float physics_time = 0.0f;
		float physics_frame = 1000.0f / 60.0f;

		while (IsRunning() && glfwWindowShouldClose(m_Window) == 0)
		{
			// poll

			glfwPollEvents();

			// update

			unsigned long time_current = GetTickCount();
			float time_delta = (float)(time_current - time_start);
			time_start = time_current;

			physics_time += time_delta;

			while (physics_time > physics_frame)
			{
				Update(1.0f);

				physics_time -= physics_frame;
			}

			// render

			Render();

			// swap

			glfwSwapBuffers(m_Window);
		}

		Destroy();
		_DestroyOpenGL();

		return 0;
	}

	void Application::OnCharacter(unsigned int a_Character)
	{
	}

	void Application::OnKeyPressed(int a_Key, int a_ScanCode, int a_Modifiers)
	{
	}

	void Application::OnKeyReleased(int a_Key, int a_ScanCode, int a_Modifiers)
	{
	}

	void Application::OnMouseMoved(const glm::vec2& a_Position)
	{
	}

	void Application::OnButtonPressed(int a_Button, const glm::vec2& a_Position, int a_Modifiers)
	{
	}

	void Application::OnButtonReleased(int a_Button, const glm::vec2& a_Position, int a_Modifiers)
	{
	}

}; // namespace Framework