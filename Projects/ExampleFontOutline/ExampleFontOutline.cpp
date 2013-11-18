// STL

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// Windows

#include <windows.h>

// Framework

#include <Application.h>
#include <DebugHelper.h>
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

#include <CurveSettings.h>
#include <Family.h>
#include <Face.h>
#include <FontFace.h>
#include <FontFreetype.h>
#include <FontLoaderFreetype.h>
#include <Library.h>
#include <LineShape.h>
#include <Polygon.h>
#include <TextLayout.h>

namespace exl = ExLibris;

class OutlineVisitor
	: public exl::ITextLayoutVisitor
{

public:

	OutlineVisitor(exl::Library* a_Library)
		: m_Library(a_Library) 
		, m_Face(nullptr)
		, m_DrawFilled(true)
		, m_DrawOutline(true)
		, m_Program(nullptr)
		, m_Position(0.0f, 0.0f)
		, m_Dimensions(0.0f, 0.0f)
	{
		m_CurveSettings.precision = 10;

		m_LineOptions.quality = exl::LineMeshOptions::eQuality_Gapless;
		m_LineOptions.thickness = 5.0f;

		m_HelperGlyphs = new fw::DebugHelper(m_Library);
		m_HelperGlyphs->SetColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	}

	~OutlineVisitor()
	{
		_ClearMeshes();

		delete m_HelperGlyphs;
	}

	void SetPosition(const glm::vec2& a_Position)
	{
		m_Position = a_Position;
	}

	void SetShaderProgram(fw::ShaderProgram* a_Program)
	{
		m_Program = a_Program;
	}

	void BuildMeshes(exl::TextLayout* a_Layout)
	{
		if (m_Face != a_Layout->GetFace())
		{
			_ClearMeshes();

			m_Face = a_Layout->GetFace();
		}

		const std::vector<ExLibris::TextLine*>& lines = a_Layout->GetLines();

		for (std::vector<ExLibris::TextLine*>::const_iterator line_it = lines.begin(); line_it != lines.end(); ++line_it)
		{
			ExLibris::TextLine* line = *line_it;

			for (std::vector<ExLibris::TextCharacter*>::const_iterator char_it = line->characters.begin(); char_it != line->characters.end(); ++char_it)
			{
				ExLibris::TextCharacter* character = *char_it;

				if (character->type == ExLibris::TextCharacter::eType_Character)
				{
					_AddMeshesForCharacter(character);
				}
			}
		}
	}

	void VisitTextBegin(const exl::FontFace* a_Face, const glm::vec2& a_Dimensions, const exl::BoundingBox& a_BoundingBox)
	{
		/*m_Face = a_Face;
		m_Dimensions = a_Dimensions;

		_ClearMeshes();

		m_HelperGlyphs->Clear();*/
	}

	void VisitTextLineBegin(size_t a_GlyphCount, const glm::vec2& a_Offset, float a_Width, const exl::BoundingBox& a_BoundingBox)
	{
	}

	void VisitTextCharacter(const exl::Glyph* a_Glyph, float a_X, float a_Advance, const exl::BoundingBox& a_BoundingBox)
	{
		/*GlyphInstance* instance = new GlyphInstance;

		m_HelperGlyphs->AddBox(a_BoundingBox.GetTranslated(m_Position));
		
		instance->position = a_BoundingBox.GetTopLeft();

		std::map<unsigned int, MeshEntry*>::iterator mesh_found = m_MeshCache.find(a_Glyph->index);
		if (mesh_found != m_MeshCache.end())
		{
			instance->meshes = mesh_found->second;
		}
		else
		{
			instance->meshes = new MeshEntry;
			instance->meshes->mesh_filled = new fw::MeshOpenGL();
			instance->meshes->mesh_outline = new fw::MeshOpenGL();

			std::vector<exl::Polygon> polygons = a_Glyph->outline->BuildPolygons(m_CurveSettings);
			if (polygons.size() > 0)
			{
				exl::LineShape shape;

				for (std::vector<exl::Polygon>::iterator poly_it = polygons.begin(); poly_it != polygons.end(); ++poly_it)
				{
					shape.AddPolygon(*poly_it);
				}

				exl::MeshBuilder* builder_filled = shape.BuildFilledMesh();
				if (builder_filled != nullptr && builder_filled->GetVertexCount() > 0)
				{
					builder_filled->Accept(*instance->meshes->mesh_filled);
				}

				if (builder_filled != nullptr)
				{
					delete builder_filled;
				}

				exl::MeshBuilder* builder_outline = shape.BuildOutlineMesh(m_LineOptions);
				if (builder_outline != nullptr && builder_outline->GetVertexCount() > 0)
				{
					builder_outline->Accept(*instance->meshes->mesh_outline);
				}

				if (builder_outline != nullptr)
				{
					delete builder_outline;
				}
			}

			m_MeshCache.insert(std::make_pair(a_Glyph->index, instance->meshes));
		}

		m_GlyphMeshes.push_back(instance);*/
	}

	void VisitTextWhitespace(unsigned int a_Identifier, float a_X, float a_Advance, const exl::BoundingBox& a_BoundingBox)
	{
	}

	void VisitTextLineEnd()
	{
	}

	void VisitTextEnd()
	{
	}

	void SetLineOptions(const exl::LineMeshOptions& a_Options)
	{
		m_LineOptions = a_Options;
	}

	void SetCurveSettings(const exl::CurveSettings& a_Settings)
	{
		m_CurveSettings = a_Settings;
	}

	void SetColorFilled(const glm::vec4& a_Color)
	{
		m_ColorFilled = a_Color;
	}

	void SetColorOutline(const glm::vec4& a_Color)
	{
		m_ColorOutline = a_Color;
	}

	bool GetDrawFilled() const
	{
		return m_DrawFilled;
	}

	void SetDrawFilled(bool a_Value)
	{
		m_DrawFilled = a_Value;
	}

	bool GetDrawOutline() const
	{
		return m_DrawOutline;
	}

	void SetDrawOutline(bool a_Value)
	{
		m_DrawOutline = a_Value;
	}

	void Render(const glm::mat4x4& a_MatrixProjection, const glm::mat4x4& a_MatrixModelView)
	{
		GLint attribute_position = m_Program->FindAttribute("attrPosition");
		GLint uniform_mvp = m_Program->FindUniform("matModelViewProjection");
		GLint uniform_color = m_Program->FindUniform("uniColor");

		glUseProgram(m_Program->GetHandle());

		glEnableVertexAttribArray(attribute_position);

		glm::mat4x4 mvp = a_MatrixProjection * a_MatrixModelView;

		for (std::vector<GlyphInstance*>::iterator instance_it = m_GlyphMeshes.begin(); instance_it != m_GlyphMeshes.end(); ++instance_it)
		{
			GlyphInstance* instance = *instance_it;

			glm::vec3 glyph_position = glm::vec3(m_Position + instance->position, 0.0f);

			glm::mat4x4 glyph_matrix = glm::translate(mvp, glyph_position);
			glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(glyph_matrix));
			
			if (m_DrawOutline)
			{
				glUniform4fv(uniform_color, 1, glm::value_ptr(m_ColorOutline));

				glBindBuffer(GL_ARRAY_BUFFER, instance->meshes->mesh_outline->GetBuffer());
				glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

				glDrawArrays(GL_TRIANGLES, 0, instance->meshes->mesh_outline->GetVertexCount());
			}
			
			if (m_DrawFilled)
			{
				glUniform4fv(uniform_color, 1, glm::value_ptr(m_ColorFilled));

				glBindBuffer(GL_ARRAY_BUFFER, instance->meshes->mesh_filled->GetBuffer());
				glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

				glDrawArrays(GL_TRIANGLES, 0, instance->meshes->mesh_filled->GetVertexCount());
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);

		m_HelperGlyphs->Render(640, 480);
	}

private:

	void _AddMeshesForCharacter(exl::TextCharacter* a_Character)
	{
		GlyphInstance* instance = new GlyphInstance;

		instance->position = a_Character->bounding_box.GetTopLeft();

		std::map<unsigned int, MeshEntry*>::iterator mesh_found = m_MeshCache.find(a_Character->identifier);
		if (mesh_found != m_MeshCache.end())
		{
			instance->meshes = mesh_found->second;
		}
		else
		{
			instance->meshes = new MeshEntry;
			instance->meshes->mesh_filled = new fw::MeshOpenGL();
			instance->meshes->mesh_outline = new fw::MeshOpenGL();

			exl::CurvePath* outline = m_Face->CreateOutline(a_Character->identifier);
			if (outline == nullptr)
			{
				return;
			}

			std::vector<exl::Polygon> polygons = outline->BuildPolygons(m_CurveSettings);
			if (polygons.size() > 0)
			{
				exl::LineShape shape;

				for (std::vector<exl::Polygon>::iterator poly_it = polygons.begin(); poly_it != polygons.end(); ++poly_it)
				{
					shape.AddPolygon(*poly_it);
				}

				exl::MeshBuilder* builder_filled = shape.BuildFilledMesh();
				if (builder_filled != nullptr && builder_filled->GetVertexCount() > 0)
				{
					builder_filled->Accept(*instance->meshes->mesh_filled);
				}

				if (builder_filled != nullptr)
				{
					delete builder_filled;
				}

				exl::MeshBuilder* builder_outline = shape.BuildOutlineMesh(m_LineOptions);
				if (builder_outline != nullptr && builder_outline->GetVertexCount() > 0)
				{
					builder_outline->Accept(*instance->meshes->mesh_outline);
				}

				if (builder_outline != nullptr)
				{
					delete builder_outline;
				}
			}

			m_MeshCache.insert(std::make_pair(a_Character->identifier, instance->meshes));
		}

		m_GlyphMeshes.push_back(instance);
	}

	void _ClearMeshes() 
	{
		for (std::map<unsigned int, MeshEntry*>::iterator entry_it = m_MeshCache.begin(); entry_it != m_MeshCache.end(); ++entry_it)
		{
			MeshEntry* entry = entry_it->second;

			delete entry->mesh_filled;
			delete entry->mesh_outline;
			delete entry;
		}
		m_MeshCache.clear();

		for (std::vector<GlyphInstance*>::iterator glyph_it = m_GlyphMeshes.begin(); glyph_it != m_GlyphMeshes.end(); ++glyph_it)
		{
			delete *glyph_it;
		}
		m_GlyphMeshes.clear();
	}

private:

	exl::Library* m_Library;
	exl::Face* m_Face;
	glm::vec2 m_Dimensions;
	glm::vec2 m_Position;

	exl::CurveSettings m_CurveSettings;
	exl::LineMeshOptions m_LineOptions;

	fw::ShaderProgram* m_Program;

	struct MeshEntry
	{
		fw::MeshOpenGL* mesh_filled;
		fw::MeshOpenGL* mesh_outline;
	};

	std::map<unsigned int, MeshEntry*> m_MeshCache;

	struct GlyphInstance
	{
		MeshEntry* meshes;
		glm::vec2 position;
	};
	std::vector<GlyphInstance*> m_GlyphMeshes;

	bool m_DrawFilled;
	glm::vec4 m_ColorFilled;

	bool m_DrawOutline;
	glm::vec4 m_ColorOutline;

	fw::DebugHelper* m_HelperGlyphs;
	bool m_HelperGlyphsVisible;

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
		, m_ProgramLines(nullptr)
		, m_ProgramTriangles(nullptr)
		, m_TextLayout(nullptr)
		, m_OutlineVisitor(nullptr)
		, m_CameraZoom(1.0f)
		, m_CameraZoomSpeed(0.0f)
		, m_Library(nullptr)
		, m_Face(nullptr)
		, m_DebugHelper(nullptr)
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

		m_CurveSettings.precision = 10;

		m_MeshOptions.quality = exl::LineMeshOptions::eQuality_Gapless;
		m_MeshOptions.thickness = 5.0f;

		m_Library = new exl::Library;
		m_Library->AddLoader(new exl::FontLoaderFreetype(m_Library));

		try
		{
			m_DebugHelper = new fw::DebugHelper(m_Library);
		}
		catch (std::exception& e)
		{
			MessageBoxA(0, e.what(), "Error while creating TextHelper", MB_OK);
			return false;
		}

		Timer timer;

		timer.Start();
		{
			m_Library->MapFontToFile("Fonts/Roboto/Roboto-Regular.ttf");
			m_Library->MapFontToFile("Fonts/Roboto/Roboto-Bold.ttf");
			m_Library->MapFontToFile("Fonts/Roboto/Roboto-Italic.ttf");
			m_Library->MapFontToFile("Fonts/Roboto/Roboto-BoldItalic.ttf");
		}
		timer.End();

		std::cout << "Loading fonts:    " << timer.GetMilliSeconds() << " ms." << std::endl;

		timer.Start();
		{
			m_Request.SetFamilyName("Roboto");
			m_Request.SetSize(100.0f);
			m_Request.SetWeight(exl::eWeight_Normal);
			m_Request.SetStyle(exl::eStyle_None);

			m_Face = m_Library->RequestFace(m_Request);
		}
		timer.End();

		std::cout << "Creating face:    " << timer.GetMilliSeconds() << " ms." << std::endl;

		m_TextLayout = new exl::TextLayout;
		m_TextLayout->SetFace(m_Face);
		m_TextLayout->SetText("Vegetables on sale");
		m_TextLayout->Layout();

		m_OutlineVisitor = new OutlineVisitor(m_Library);
		m_OutlineVisitor->SetColorFilled(glm::vec4(1.0f, 1.0, 0.0f, 1.0f));
		m_OutlineVisitor->SetColorOutline(glm::vec4(0.0f, 1.0, 0.0f, 1.0f));
		m_OutlineVisitor->SetShaderProgram(m_ProgramTriangles);
		m_OutlineVisitor->SetPosition(glm::vec2(100.0f, 100.0f));
		m_OutlineVisitor->BuildMeshes(m_TextLayout);

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

		m_OutlineVisitor->Render(projection, modelview);

		m_DebugHelper->Clear();

		glm::dvec2 mouse_position;
		glfwGetCursorPos(GetWindow(), &mouse_position.x, &mouse_position.y);

		char mouse_text[256] = { 0 };
		sprintf(mouse_text, "Mouse position: (%.2f, %.2f)", mouse_position.x, mouse_position.y);
		m_DebugHelper->AddText(mouse_text, glm::vec2(10.0f, 460.0f));

		_DrawInstructions();

		m_DebugHelper->Render(width, height);
	}

	void Destroy()
	{
		if (m_ProgramTriangles != nullptr)
		{
			delete m_ProgramTriangles;
		}

		if (m_ProgramLines != nullptr)
		{
			delete m_ProgramLines;
		}

		delete m_DebugHelper;
		delete m_TextLayout;
		delete m_OutlineVisitor;
		delete m_Face;
		delete m_Library;
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

		case GLFW_KEY_KP_SUBTRACT:
		case GLFW_KEY_MINUS:
			{
				m_CameraZoomSpeed = -0.05f;

			} break;

		case GLFW_KEY_KP_ADD:
		case GLFW_KEY_EQUAL:
			{
				m_CameraZoomSpeed = 0.05f;

			} break;

		case GLFW_KEY_B:
			{
				if (a_Modifiers & GLFW_MOD_CONTROL)
				{
					if (m_Request.GetWeight() == exl::eWeight_Normal)
					{
						m_Request.SetWeight(exl::eWeight_Bold);
					}
					else
					{
						m_Request.SetWeight(exl::eWeight_Normal);
					}

					m_Face = m_Library->RequestFace(m_Request);

					m_TextLayout->SetFace(m_Face);
					m_TextLayout->Layout();
					m_OutlineVisitor->BuildMeshes(m_TextLayout);
				}

			} break;

		case GLFW_KEY_I:
			{
				if (a_Modifiers & GLFW_MOD_CONTROL)
				{
					if (m_Request.GetStyle() == exl::eStyle_None)
					{
						m_Request.SetStyle(exl::eStyle_Italicized);
					}
					else
					{
						m_Request.SetStyle(exl::eStyle_None);
					}

					m_Face = m_Library->RequestFace(m_Request);
					
					m_TextLayout->SetFace(m_Face);
					m_TextLayout->Layout();
					m_OutlineVisitor->BuildMeshes(m_TextLayout);
				}

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

				if (m_OptionDrawLines)
				{
					m_OutlineVisitor->SetShaderProgram(m_ProgramLines);
				}
				else
				{
					m_OutlineVisitor->SetShaderProgram(m_ProgramTriangles);
				}

			} break;

		case GLFW_KEY_2:
			{
				m_OutlineVisitor->SetDrawFilled(!m_OutlineVisitor->GetDrawFilled());

			} break;

		case GLFW_KEY_3:
			{
				m_OutlineVisitor->SetDrawOutline(!m_OutlineVisitor->GetDrawOutline());

			} break;

		case GLFW_KEY_L:
			{
				if (m_MeshOptions.quality == ExLibris::LineMeshOptions::eQuality_Fast)
				{
					m_MeshOptions.quality = ExLibris::LineMeshOptions::eQuality_Gapless;
				}
				else if (m_MeshOptions.quality == ExLibris::LineMeshOptions::eQuality_Gapless)
				{
					m_MeshOptions.quality = ExLibris::LineMeshOptions::eQuality_Fast;
				}

				m_OutlineVisitor->SetLineOptions(m_MeshOptions);
				m_OutlineVisitor->BuildMeshes(m_TextLayout);

			} break;

		case GLFW_KEY_O:
			{
				m_CurveSettings.precision -= 5;

				m_OutlineVisitor->SetCurveSettings(m_CurveSettings);
				m_OutlineVisitor->BuildMeshes(m_TextLayout);

			} break;

		case GLFW_KEY_P:
			{
				m_CurveSettings.precision += 5;

				m_OutlineVisitor->SetCurveSettings(m_CurveSettings);
				m_OutlineVisitor->BuildMeshes(m_TextLayout);

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

		case GLFW_KEY_KP_SUBTRACT:
		case GLFW_KEY_MINUS:
		case GLFW_KEY_KP_ADD:
		case GLFW_KEY_EQUAL:
			{
				m_CameraZoomSpeed = 0.0f;

			} break;

		}
	}

private:

	void _DrawInstructions()
	{
		std::stringstream text;

		text
			<< "<Arrow keys> - Move\n"
			<< "<+>/<-> - Zoom\n"
			<< "\n"
			<< "<1> - Draw triangles or lines\n"
			<< "<2> - Filled on/off\n"
			<< "<3> - Outline on/off\n"
			<< "\n"
			<< "<L> - Toggle outline quality\n"
			<< "<O><P> - Outline precision\n"
			<< "\n"
			<< "<Ctrl> + <B> - Bold\n"
			<< "<Ctrl> + <I> - Italic\n";

		m_DebugHelper->AddText(text.str(), glm::vec2(20.0f, 20.0f));
	}

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

	exl::Library* m_Library;
	exl::Face* m_Face;
	exl::FaceRequest m_Request;
	fw::DebugHelper* m_DebugHelper;

	exl::TextLayout* m_TextLayout;
	OutlineVisitor* m_OutlineVisitor;

	exl::CurveSettings m_CurveSettings;
	exl::LineMeshOptions m_MeshOptions;

	fw::ShaderProgram* m_ProgramTriangles;
	fw::ShaderProgram* m_ProgramLines;

	glm::vec3 m_CameraPosition;
	glm::vec3 m_CameraVelocity;
	float m_CameraZoom;
	float m_CameraZoomSpeed;

	bool m_OptionDrawLines;

}; // class ExampleFontOutline

int main(int argc, const char** argv)
{
	ExampleFontOutline application(argc, argv);
	return application.Run();
}