#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Framework
{

	class Application
	{
	
	public:
	
		Application(int a_ArgumentCount, const char** a_Arguments);
		virtual ~Application();

		GLFWwindow* GetWindow() const;

		virtual bool ParseCommandLine(int a_ArgumentCount, const char** a_Arguments) = 0;

		virtual bool Initialize() = 0;
		virtual void Update(float a_DeltaTime) = 0;
		virtual void Render() = 0;
		virtual void Destroy() = 0;

		bool IsRunning() const;
		void Quit();

		int Run();

	protected:

		virtual void OnKeyPressed(int a_Key, int a_ScanCode, int a_Modifiers);
		virtual void OnKeyReleased(int a_Key, int a_ScanCode, int a_Modifiers);

		virtual void OnMouseMoved(const glm::vec2& a_Position);
		virtual void OnButtonPressed(int a_Button, const glm::vec2& a_Position, int a_Modifiers);
		virtual void OnButtonReleased(int a_Button, const glm::vec2& a_Position, int a_Modifiers);

	private:

		bool _InitializeOpenGL();
		void _DestroyOpenGL();

		friend void ProcessKey(GLFWwindow* a_Window, int a_Key, int a_ScanCode, int a_Action, int a_Modifiers);
		friend void ProcessButton(GLFWwindow* a_Window, int a_Button, int a_Action, int a_Modifiers);
		friend void ProcessCursor(GLFWwindow* a_Window, double a_X, double a_Y);

	protected:

		bool m_Running;

		int m_ArgumentCount;
		const char** m_Arguments;

		GLFWwindow* m_Window;
	
	}; // class Application

}; // namespace Framework

#endif