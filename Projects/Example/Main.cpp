#if defined(_WINDOWS) && defined(_DEBUG)
#	include <vld.h>
#endif

// STL

#include <iostream>
#include <map>
#include <string>
#include <vector>

// Framework

#include <Application.h>
#include <FontSystem.h>
#include <MeshOpenGL.h>
#include <ShaderLoader.h>
#include <ShaderProgram.h>

namespace fw = Framework;

// GLM

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ExLibris

#include <FontFace.h>
#include <FontFreetype.h>
#include <FontLoaderFreetype.h>
#include <Library.h>
#include <TextHelper.h>
#include <TextLayout.h>

namespace exl = ExLibris;

class Example
	: public fw::Application
{

public:

	Example(int a_ArgumentCount, const char** a_Arguments)
		: fw::Application(a_ArgumentCount, a_Arguments)
		, m_Library(nullptr)
		, m_Font(nullptr)
		, m_Face(nullptr)
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
		m_ShaderLoader = new fw::ShaderLoader();

		m_Library = new exl::Library;
		m_Library->AddLoader(new exl::FontLoaderFreetype(m_Library));

		m_TextHelper = new fw::TextHelper(m_Library, m_ShaderLoader);
		m_TextHelper->AddText("De kat krabt de krullen van de trap.", glm::vec2(20.0f, 20.0f));

		return true;
	}

	void Update(float a_DeltaTime)
	{
	}

	void Render()
	{
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

		m_TextHelper->Render(projection);
	}

	void Destroy()
	{
		delete m_TextHelper;
		delete m_Library;
		delete m_ShaderLoader;
	}

private:

	fw::ShaderLoader* m_ShaderLoader;
	exl::Library* m_Library;
	exl::IFont* m_Font;
	exl::FontFace* m_Face;
	fw::TextHelper* m_TextHelper;

}; // class Example

int main(int argc, const char** argv)
{
	Example application(argc, argv);
	return application.Run();

	return 0;
}