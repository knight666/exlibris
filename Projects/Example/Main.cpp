// STL

#include <iostream>
#include <map>
#include <string>
#include <vector>

// Windows

#include <windows.h>

// Framework

#include <Application.h>

namespace fw = Framework;

// ExLibris

#include <DebugHelper.h>

namespace exl = ExLibris;

class Example
	: public fw::Application
{

public:

	Example(int a_ArgumentCount, const char** a_Arguments)
		: fw::Application(a_ArgumentCount, a_Arguments)
	{
	}

	~Example()
	{
	}

	bool ParseCommandLine(int a_ArgumentCount, const char** a_Arguments)
	{
		return true;
	}

	bool Initialize()
	{
		try
		{
			m_DebugHelper = new fw::DebugHelper;
		}
		catch (std::exception& e)
		{
			MessageBoxA(0, e.what(), "Error while creating TextHelper", MB_OK);
			return false;
		}
		
		glm::vec4 color(1.0f, 1.0f, 0.0f, 1.0f);
		m_DebugHelper->AddText("Debug text can be rendered without any external dependencies.", glm::vec2(20.0f, 20.0f), color);

		m_DebugHelper->AddText("ASCII table:\n\n !\"#$%&'()*+,-./0123456789:;<=>?\n@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\n`abcdefghijklmnopqrstuvwxyz{|}~\xfe", glm::vec2(20.0f, 80.0f), color);

		return true;
	}

	void Update(float a_DeltaTime)
	{
	}

	void Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int width, height;
		glfwGetFramebufferSize(GetWindow(), &width, &height);

		m_DebugHelper->Render(width, height);
	}

	void Destroy()
	{
		delete m_DebugHelper;
	}

private:

	fw::DebugHelper* m_DebugHelper;

}; // class Example

int main(int argc, const char** argv)
{
	Example application(argc, argv);
	return application.Run();

	return 0;
}