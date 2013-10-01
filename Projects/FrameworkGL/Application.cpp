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

	static void ProcessKey(GLFWwindow* a_Window, int a_Key, int a_ScanCode, int a_Action, int a_Modifiers)
	{
		Application* application = (Application*)glfwGetWindowUserPointer(a_Window);

		if (a_Action == GLFW_PRESS)
		{
			application->OnKeyPressed(a_Key, a_ScanCode, a_Modifiers);
		}
		else if (a_Action == GLFW_PRESS)
		{
			application->OnKeyReleased(a_Key, a_ScanCode, a_Modifiers);
		}
	}

	void ProcessButton(GLFWwindow* a_Window, int a_Button, int a_Action, int a_Modifiers)
	{
		Application* application = (Application*)glfwGetWindowUserPointer(a_Window);

		if (a_Action == GLFW_PRESS)
		{
			application->OnButtonPressed(a_Button, a_Modifiers);
		}
		else if (a_Action == GLFW_RELEASE)
		{
			application->OnButtonReleased(a_Button, a_Modifiers);
		}
	}

	void ProcessCursor(GLFWwindow* a_Window, double a_X, double a_Y)
	{
		Application* application = (Application*)glfwGetWindowUserPointer(a_Window);

		glm::vec2 position((float)a_X, (float)a_Y);

		application->OnMouseMoved(position);
	}

	Application::Application(int a_ArgumentCount, const char** a_Arguments)
		: m_Running(false)
	{
		ParseCommandLine(a_ArgumentCount, a_Arguments);
	}
	
	Application::~Application()
	{
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

	int Application::Run()
	{
		if (!_InitializeOpenGL())
		{
			glfwTerminate();

			return -1;
		}

		if (!Initialize())
		{
			return -1;
		}

		unsigned long time_start = GetTickCount();
		float physics_time = 0.0f;
		float physics_frame = 1000.0f / 60.0f;

		while (IsRunning())
		{
			unsigned long time_current = GetTickCount();
			float time_delta = (float)(time_current - time_start);
			time_start = time_current;

			physics_time += time_delta;

			// update

			while (physics_time > physics_frame)
			{
				Update(1.0f);

				physics_time -= physics_frame;
			}

			// render

			Render();
		}

		return 0;
	}

	void Application::OnKeyPressed(int a_Key, int a_ScanCode, int a_Modifiers)
	{
	}

	void Application::OnKeyReleased(int a_Key, int a_ScanCode, int a_Modifiers)
	{
	}

	void Application::OnButtonPressed(int a_Button, int a_Modifiers)
	{
	}

	void Application::OnButtonReleased(int a_Button, int a_Modifiers)
	{
	}

	void Application::OnMouseMoved(const glm::vec2& a_Position)
	{
	}

}; // namespace Framework