// STL

#include <iostream>
#include <map>
#include <string>
#include <vector>

// Framework

#include <Application.h>
#include <MeshOpenGL.h>
#include <ShaderLoader.h>
#include <ShaderProgram.h>

namespace fw = Framework;

// GLM

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OpenGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// ExLibris

#include <CurveSettings.h>
#include <FontFace.h>
#include <FontFreetype.h>
#include <FontLoaderFreetype.h>
#include <LineShape.h>
#include <Polygon.h>
#include <TextLayout.h>

namespace exl = ExLibris;

class OutlineVisitor
	: public exl::ITextLayoutVisitor
{

public:

	OutlineVisitor()
	{
	}

	~OutlineVisitor()
	{
	}

	void VisitTextBegin(const exl::FontFace* a_Face, const glm::vec2& a_Dimensions)
	{
		m_Face = a_Face;
		m_Dimensions = a_Dimensions;
	}

	void VisitTextLineBegin(size_t a_GlyphCount, const glm::vec2& a_Offset, float a_Width)
	{
		m_LineOffset = a_Offset;
	}

	void VisitTextCharacter(const exl::Glyph* a_Glyph, float a_X, float a_Advance)
	{
		GlyphInstance* instance = new GlyphInstance;
		
		instance->position = m_LineOffset;
		instance->position.x += a_X;

		std::map<unsigned int, fw::MeshOpenGL*>::iterator mesh_found = m_MeshCache.find(a_Glyph->index);
		if (mesh_found != m_MeshCache.end())
		{
			instance->mesh = mesh_found->second;
		}
		else
		{
			instance->mesh = new fw::MeshOpenGL();

			exl::CurveSettings curve_settings;
			curve_settings.precision = 10;

			exl::LineMeshOptions line_mesh_options;
			line_mesh_options.quality = exl::LineMeshOptions::eQuality_Gapless;
			line_mesh_options.thickness = 5.0f;

			std::vector<exl::Polygon> polygons = a_Glyph->outline->BuildPolygons(curve_settings);
			if (polygons.size() > 0)
			{
				exl::LineShape shape;

				for (std::vector<exl::Polygon>::iterator poly_it = polygons.begin(); poly_it != polygons.end(); ++poly_it)
				{
					shape.AddPolygon(*poly_it);
				}

				//exl::MeshBuilder* builder = shape.BuildMesh(line_mesh_options);
				exl::MeshBuilder* builder = shape.BuildFilledMesh();

				if (builder != nullptr && builder->GetVertexCount() > 0)
				{
					builder->Accept(*instance->mesh);
					delete builder;
				}
			}

			m_MeshCache.insert(std::make_pair(a_Glyph->index, instance->mesh));
		}

		m_GlyphMeshes.push_back(instance);
	}

	void VisitTextWhitespace(float a_X, float a_Advance)
	{
	}

	void VisitTextLineEnd()
	{
	}

	void VisitTextEnd()
	{
	}

	void Render(fw::ShaderProgram* a_ShaderProgram, const glm::mat4x4& a_MatrixProjection, const glm::mat4x4& a_MatrixModelView, GLint a_AttributePosition)
	{
		for (std::vector<GlyphInstance*>::iterator instance_it = m_GlyphMeshes.begin(); instance_it != m_GlyphMeshes.end(); ++instance_it)
		{
			GlyphInstance* instance = *instance_it;

			glm::mat4x4 offset;
			offset = glm::translate(offset, glm::vec3(instance->position.x, instance->position.y, 0.0f));

			glm::mat4x4 mvp = a_MatrixProjection * a_MatrixModelView * offset;

			glUniformMatrix4fv(a_ShaderProgram->GetUniform("matModelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));

			glBindBuffer(GL_ARRAY_BUFFER, instance->mesh->GetBuffer());
			glVertexAttribPointer(a_AttributePosition, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

			glDrawArrays(GL_TRIANGLES, 0, instance->mesh->GetVertexCount());
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}

private:

	const exl::FontFace* m_Face;
	glm::vec2 m_Dimensions;

	glm::vec2 m_LineOffset;

	std::map<unsigned int, fw::MeshOpenGL*> m_MeshCache;

	struct GlyphInstance
	{
		fw::MeshOpenGL* mesh;
		glm::vec2 position;
	};
	std::vector<GlyphInstance*> m_GlyphMeshes;

}; // class OutlineVisitor

class Timer
{

public:

	void Start()
	{
		m_TimeStart = (float)glfwGetTime();
	}

	void End()
	{
		m_DifferenceSeconds = (float)glfwGetTime() - m_TimeStart;
	}

	float GetMilliSeconds() const
	{
		return m_DifferenceSeconds * 1000.0f;
	}

private:

	float m_TimeStart;
	float m_DifferenceSeconds;

};

class ExampleFontOutline
	: public fw::Application
{

public:

	ExampleFontOutline(int a_ArgumentCount, const char** a_Arguments)
		: fw::Application(a_ArgumentCount, a_Arguments)
		, m_OptionDrawLines(false)
		, m_ShaderLoader(nullptr)
		, m_ProgramLines(nullptr)
		, m_ProgramTriangles(nullptr)
		, m_Mesh(nullptr)
		, m_TextLayout(nullptr)
		, m_OutlineVisitor(nullptr)
		, m_CameraZoom(1.0f)
		, m_CameraZoomSpeed(0.0f)
	{
	}

	bool ParseCommandLine(int a_ArgumentCount, const char** a_Arguments)
	{
		m_FontPath = "Fonts/Roboto/Roboto-Regular.ttf";
		//m_FontPath = "Fonts/Mathilde/mathilde.otf";
		m_FontSize = 100.0f;

		return true;
	}

	bool Initialize()
	{
		m_ShaderLoader = new fw::ShaderLoader();
		_LoadShaders();

		m_CurveSettings.precision = 10;

		m_MeshOptions.quality = exl::LineMeshOptions::eQuality_Gapless;
		m_MeshOptions.thickness = 5.0f;

		m_Mesh = new fw::MeshOpenGL();

		m_FontLoader = new exl::FontLoaderFreetype();

		Timer timer;

		timer.Start();
		{
			m_Font = m_FontLoader->LoadFont(m_FontPath);
		}
		timer.End();

		std::cout << "Loading font:     " << timer.GetMilliSeconds() << " ms." << std::endl;

		timer.Start();
		{
			m_FontFace = m_Font->CreateFace(m_FontSize);
		}
		timer.End();

		std::cout << "Creating face:    " << timer.GetMilliSeconds() << " ms." << std::endl;

		timer.Start();
		{
			_BuildGlyphMesh();

		}
		timer.End();

		std::cout << "Creating outline: " << timer.GetMilliSeconds() << " ms." << std::endl;

		m_TextLayout = new exl::TextLayout;
		m_TextLayout->SetFontFace(m_FontFace);
		m_TextLayout->SetText("Printing some text...");

		m_OutlineVisitor = new OutlineVisitor;
		m_TextLayout->Accept(*m_OutlineVisitor);

		return true;
	}

	void Update(float a_DeltaTime)
	{
		m_CameraPosition += m_CameraVelocity * a_DeltaTime;
		m_CameraZoom += m_CameraZoomSpeed * a_DeltaTime;
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

		glm::mat4x4 modelview;
		modelview = glm::translate(modelview, m_CameraPosition);
		modelview = glm::scale(modelview, glm::vec3(m_CameraZoom, m_CameraZoom, 1.0f));

		//glm::mat4x4 mvp = projection * modelview;

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
		glUniform4fv(outline_program->GetUniform("uniColor"), 1, glm::value_ptr(outline_color));

		GLint attribute_position = outline_program->GetAttribute("attrPosition");
		glEnableVertexAttribArray(attribute_position);

		/*glBindBuffer(GL_ARRAY_BUFFER, m_Mesh->GetBuffer());
		glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glDrawArrays(GL_TRIANGLES, 0, m_Mesh->GetVertexCount());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);*/

		m_OutlineVisitor->Render(outline_program, projection, modelview, attribute_position);
	}

	void Destroy()
	{
		delete m_Mesh;
		delete m_Glyph;
		delete m_FontFace;
		delete m_Font;
		delete m_FontLoader;
	}

private:

	void OnKeyPressed(int a_Key, int a_ScanCode, int a_Modifiers)
	{
		float speed = 5.0f;

		switch (a_Key)
		{

		case GLFW_KEY_LEFT:
			{
				m_CameraVelocity.x = speed;

			} break;

		case GLFW_KEY_RIGHT:
			{
				m_CameraVelocity.x = -speed;

			} break;

		case GLFW_KEY_UP:
			{
				m_CameraVelocity.y = speed;

			} break;

		case GLFW_KEY_DOWN:
			{
				m_CameraVelocity.y = -speed;

			} break;

		case GLFW_KEY_MINUS:
			{
				m_CameraZoomSpeed = -0.05f;

			} break;

		case GLFW_KEY_EQUAL:
			{
				m_CameraZoomSpeed = 0.05f;

			} break;

		}
	}

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

		case GLFW_KEY_1:
			{
				m_OptionDrawLines = !m_OptionDrawLines;

			} break;

		case GLFW_KEY_2:
			{
				if (m_MeshOptions.quality == ExLibris::LineMeshOptions::eQuality_Fast)
				{
					m_MeshOptions.quality = ExLibris::LineMeshOptions::eQuality_Gapless;
				}
				else if (m_MeshOptions.quality == ExLibris::LineMeshOptions::eQuality_Gapless)
				{
					m_MeshOptions.quality = ExLibris::LineMeshOptions::eQuality_Fast;
				}

				_BuildGlyphMesh();

			} break;

		case GLFW_KEY_O:
			{
				m_CurveSettings.precision -= 5;

				_BuildGlyphMesh();

			} break;

		case GLFW_KEY_P:
			{
				m_CurveSettings.precision += 5;

				_BuildGlyphMesh();

			} break;

		case GLFW_KEY_LEFT:
		case GLFW_KEY_RIGHT:
			{
				m_CameraVelocity.x = 0.0f;

			} break;

		case GLFW_KEY_UP:
		case GLFW_KEY_DOWN:
			{
				m_CameraVelocity.y = 0.0f;

			} break;

		case GLFW_KEY_MINUS:
		case GLFW_KEY_EQUAL:
			{
				m_CameraZoomSpeed = 0.0f;

			} break;

		}
	}


private:

	void _LoadShaders()
	{
		if (m_ProgramTriangles != nullptr)
		{
			delete m_ProgramTriangles;
		}

		m_ProgramTriangles = m_ShaderLoader->LoadProgram("Triangles2D", "Shaders/Triangles2D");
		m_ProgramTriangles->Compile();
		m_ProgramTriangles->Link();

		if (m_ProgramLines != nullptr)
		{
			delete m_ProgramLines;
		}

		m_ProgramLines = m_ShaderLoader->LoadProgram("Triangles2D", "Shaders/Lines2D");
		m_ProgramLines->Compile();
		m_ProgramLines->Link();
	}

	void _BuildGlyphMesh() 
	{
		m_Glyph = m_FontFace->FindGlyph((unsigned int)L'A');

		std::vector<exl::Polygon> polygons = m_Glyph->outline->BuildPolygons(m_CurveSettings);
		if (polygons.size() > 0)
		{
			m_Shape.Clear();

			for (std::vector<exl::Polygon>::iterator poly_it = polygons.begin(); poly_it != polygons.end(); ++poly_it)
			{
				m_Shape.AddPolygon(*poly_it);
			}

			exl::MeshBuilder* builder = m_Shape.BuildOutlineMesh(m_MeshOptions);
			if (builder != nullptr && builder->GetVertexCount() > 0)
			{
				builder->Accept(*m_Mesh);
				delete builder;
			}
		}
	}

private:

	bool m_OptionDrawLines;

	std::string m_FontPath;
	float m_FontSize;

	exl::FontLoaderFreetype* m_FontLoader;
	exl::IFont* m_Font;
	exl::FontFace* m_FontFace;

	exl::TextLayout* m_TextLayout;
	OutlineVisitor* m_OutlineVisitor;

	exl::Glyph* m_Glyph;
	exl::LineShape m_Shape;
	exl::CurveSettings m_CurveSettings;
	exl::LineMeshOptions m_MeshOptions;

	fw::ShaderLoader* m_ShaderLoader;
	fw::ShaderProgram* m_ProgramTriangles;
	fw::ShaderProgram* m_ProgramLines;
	fw::MeshOpenGL* m_Mesh;

	glm::vec3 m_CameraPosition;
	glm::vec3 m_CameraVelocity;
	float m_CameraZoom;
	float m_CameraZoomSpeed;

}; // class ExampleFontOutline

int main(int argc, const char** argv)
{
	ExampleFontOutline application(argc, argv);
	return application.Run();
}