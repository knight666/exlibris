#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Framework
{

	class Application
	{
	
	public:
	
		Application(int a_ArgumentCount, const char** a_Arguments);
		virtual ~Application();

		virtual bool ParseCommandLine(int a_ArgumentCount, const char** a_Arguments) = 0;

		virtual bool Initialize() = 0;
		virtual void Update(float a_DeltaTime) = 0;
		virtual void Render() = 0;

		bool IsRunning() const;
		void Quit();

		int Run();

	protected:

		virtual void OnKeyPressed(int a_Key, int a_ScanCode, int a_Modifiers);
		virtual void OnKeyReleased(int a_Key, int a_ScanCode, int a_Modifiers);

		virtual void OnButtonPressed(int a_Button, int a_Modifiers);
		virtual void OnButtonReleased(int a_Button, int a_Modifiers);

		virtual void OnMouseMoved(const glm::vec2& a_Position);

	private:

		bool _InitializeOpenGL();

		friend void ProcessKey(GLFWwindow* a_Window, int a_Key, int a_ScanCode, int a_Action, int a_Modifiers);
		friend void ProcessButton(GLFWwindow* a_Window, int a_Button, int a_Action, int a_Modifiers);
		friend void ProcessCursor(GLFWwindow* a_Window, double a_X, double a_Y);

	protected:

		bool m_Running;

		GLFWwindow* m_Window;
	
	}; // class Application

}; // namespace Framework

#endif