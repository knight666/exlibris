﻿// STL

#include <iostream>
#include <string>
#include <map>
#include <vector>

// Windows

#include <windows.h>

// Freetype

#include <freetype/ftbbox.h>
#include <freetype/ftoutln.h>

// GLM

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OpenGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Poly2Tri

#include <poly2tri/poly2tri.h>

// Framework

#include <ShaderLoader.h>
#include <ShaderProgram.h>
#include <ShaderSource.h>

// ExLibris

#include <FontFace.h>
#include <FontLoaderFreetype.h>
#include <Glyph.h>
#include <IMeshVisitor.h>
#include <LineShape.h>

// Options

static bool g_LoadFont = false;
//static std::string g_FontPath = "Fonts/Mathilde/mathilde.otf";
static std::string g_FontPath = "Fonts/Roboto/Roboto-Regular.ttf";
static float g_FontSize = 100.0f;
static std::wstring g_Text = L"Pa's wijze lynx bezag vroom het fikse aquaduct";
//static std::wstring g_Text = L"agjklipqsdf";

static Framework::ShaderLoader* g_ShaderLoader;
static Framework::ShaderProgram* g_ShaderProgram = nullptr;
static Framework::ShaderProgram* g_ProgramOutlineTriangles = nullptr;
static Framework::ShaderProgram* g_ProgramOutlineLines = nullptr;

static bool g_DrawLines = false;

static void OnGlfwError(int error, const char* description)
{
	std::cerr << description << std::endl;
}

void LoadShaders()
{
	if (g_ShaderProgram != nullptr)
	{
		delete g_ShaderProgram;
	}

	g_ShaderProgram = g_ShaderLoader->LoadProgram("Default", "Shaders/Default");
	g_ShaderProgram->Compile();
	g_ShaderProgram->Link();

	if (g_ProgramOutlineTriangles != nullptr)
	{
		delete g_ProgramOutlineTriangles;
	}

	g_ProgramOutlineTriangles = g_ShaderLoader->LoadProgram("Triangles2D", "Shaders/Triangles2D");
	g_ProgramOutlineTriangles->Compile();
	g_ProgramOutlineTriangles->Link();

	if (g_ProgramOutlineLines != nullptr)
	{
		delete g_ProgramOutlineLines;
	}

	g_ProgramOutlineLines = g_ShaderLoader->LoadProgram("Lines2D", "Shaders/Lines2D");
	g_ProgramOutlineLines->Compile();
	g_ProgramOutlineLines->Link();
}

static glm::vec3 g_CameraPosition;
static float g_CameraZoom = 1.0f;

static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		float speed = 5.0f;

		if (key == GLFW_KEY_LEFT)
		{
			g_CameraPosition.x -= speed;
		}
		else if (key == GLFW_KEY_RIGHT)
		{
			g_CameraPosition.x += speed;
		}
		else if (key == GLFW_KEY_UP)
		{
			g_CameraPosition.y -= speed;
		}
		else if (key == GLFW_KEY_DOWN)
		{
			g_CameraPosition.y += speed;
		}
		else if (key == GLFW_KEY_KP_ADD)
		{
			g_CameraZoom += 0.25f;
		}
		else if (key == GLFW_KEY_KP_SUBTRACT)
		{
			g_CameraZoom -= 0.25f;
		}
		else if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
	else if (key == GLFW_KEY_F5 && action == GLFW_RELEASE)
	{
		LoadShaders();
	}
	else if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
	{
		g_DrawLines = !g_DrawLines;
	}
}

static ExLibris::Polygon g_MousePath;

static void OnMouseButton(GLFWwindow* a_Window, int a_Button, int a_Action, int a_Modifiers)
{
	if (a_Button == GLFW_MOUSE_BUTTON_1 && a_Action == GLFW_RELEASE)
	{
		double cursor_position[2];
		glfwGetCursorPos(a_Window, &cursor_position[0], &cursor_position[1]);

		g_MousePath += glm::vec2((float)cursor_position[0], (float)cursor_position[1]);
	}
}

struct GlyphBitmap
{
	unsigned char* pixels;
	unsigned int width;
	unsigned int height;
};

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

/*GLuint CreateTexture(const std::map<unsigned int, Glyph*>& a_Glyphs, unsigned int a_LineHeight, const std::wstring& a_Text)
{
	unsigned int texture_width = 0;
	unsigned int texture_height = a_LineHeight;
	std::vector<Glyph*> glyphs;

	for (std::wstring::const_iterator char_it = a_Text.begin(); char_it != a_Text.end(); ++char_it)
	{
		unsigned int codepoint = (unsigned int)*char_it;

		std::map<unsigned int, Glyph*>::const_iterator glyph_found = a_Glyphs.find(codepoint);
		if (glyph_found != a_Glyphs.end())
		{
			Glyph* glyph = glyph_found->second;

			texture_width += (unsigned int)glyph->metrics->advance;

			glyphs.push_back(glyph);
		}
	}

	unsigned char* texture_data = new unsigned char[texture_width * texture_height * 4];
	memset(texture_data, 0, texture_width * texture_height * 4);

	unsigned int dst_pitch = texture_width * 4;
	unsigned int cursor_x = 0;

	for (std::vector<Glyph*>::iterator glyph_it = glyphs.begin(); glyph_it != glyphs.end(); ++glyph_it)
	{
		Glyph* glyph = *glyph_it;

		ExLibris::GlyphMetrics* metrics = glyph->metrics;
		GlyphBitmap* bitmap = glyph->bitmap;

		unsigned char* dst = texture_data + (unsigned int)(dst_pitch * metrics->offset.y) + (unsigned int)(metrics->offset.x * 4) + (cursor_x * 4);

		unsigned char* src = bitmap->pixels;
		unsigned int src_pitch = bitmap->width * 4;

		for (unsigned int y = 0; y < bitmap->height; ++y)
		{
			memcpy(dst, src, src_pitch);

			dst += dst_pitch;
			src += src_pitch;
		}

		cursor_x += (unsigned int)metrics->advance;
	}

	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		texture_width, texture_height,
		0,
		GL_BGRA, GL_UNSIGNED_BYTE, texture_data
	);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}*/

struct TextGlyphOutline
{
	unsigned int start;
	unsigned int count;
};

struct TextOutline
{
	GLuint vertex_buffer;
	unsigned int vertex_count;
	std::vector<TextGlyphOutline> contours;
};

struct TextGlyphMesh
{
	GLuint vertex_buffer;
	unsigned int vertex_count;
	glm::vec2 line_offset;
	glm::vec2 glyph_offset;
};

struct TextMesh
{
	glm::vec2 dimensions;
	std::vector<TextGlyphMesh*> glyphs;
};

TextOutline CreateTextOutline(ExLibris::FontFace* a_Face, const std::wstring& a_Text)
{
	TextOutline outline;
	outline.vertex_buffer = 0;
	outline.vertex_count = 0;

	std::vector<ExLibris::Glyph*> glyphs;

	for (std::wstring::const_iterator char_it = a_Text.begin(); char_it != a_Text.end(); ++char_it)
	{
		unsigned int character = (unsigned int)*char_it;

		ExLibris::Glyph* glyph = a_Face->FindGlyph(character);
		if (glyph == nullptr)
		{
			continue;
		}

		if (glyph->outline != nullptr)
		{
			/*for (std::vector<ExLibris::GlyphContour*>::iterator contour_it = glyph->outline->contours.begin(); contour_it != glyph->outline->contours.end(); ++contour_it)
			{
				ExLibris::GlyphContour* contour = *contour_it;

				outline.vertex_count += contour->points.size();
			}*/
		}

		glyphs.push_back(glyph);
	}

	if (outline.vertex_count == 0)
	{
		return outline;
	}

	glm::vec2 cursor_offset;

	glm::vec2* position_data = new glm::vec2[outline.vertex_count];
	unsigned int position_index = 0;

	std::vector<ExLibris::Glyph*>::iterator glyph_next_it = glyphs.begin() + 1;

	for (std::vector<ExLibris::Glyph*>::iterator glyph_it = glyphs.begin(); glyph_it != glyphs.end(); ++glyph_it)
	{
		ExLibris::Glyph* glyph = *glyph_it;

		if (glyph->outline != nullptr)
		{
			glm::vec2 position_local = cursor_offset;
			position_local.y += glyph->metrics->offset.y;

			if (glyph_next_it != glyphs.end())
			{
				ExLibris::Glyph* glyph_next = *glyph_next_it;

				glm::vec2 kerning;
				if (a_Face->TryGetKerning(glyph, glyph_next, kerning))
				{
					position_local += kerning;
				}
			}

			/*for (std::vector<ExLibris::GlyphContour*>::iterator contour_it = glyph->outline->contours.begin(); contour_it != glyph->outline->contours.end(); ++contour_it)
			{
				ExLibris::GlyphContour* contour = *contour_it;

				TextGlyphOutline glyph_contour;
				glyph_contour.start = position_index;

				std::vector<glm::vec2>::iterator position_it = contour->points.begin();
				while (position_it != contour->points.end())
				{
					position_data[position_index] = *position_it + position_local;

					++position_index;
					++position_it;
				}

				glyph_contour.count = position_index - glyph_contour.start;

				outline.contours.push_back(glyph_contour);
			}*/
		}
		
		cursor_offset.x += glyph->metrics->advance;

		if (glyph_next_it != glyphs.end())
		{
			glyph_next_it++;
		}
	}

	glGenBuffers(1, &outline.vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, outline.vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, outline.vertex_count * sizeof(glm::vec2), position_data, GL_STATIC_DRAW);

	delete [] position_data;

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return outline;
}

/*std::vector<p2t::Point*> ConvertContourToPolyline(const glm::vec2& a_Cursor, ExLibris::GlyphContour* a_Contour)
{
	std::vector<p2t::Point*> polyline;

	for (std::vector<glm::vec2>::iterator point_it = a_Contour->points.begin(); point_it != a_Contour->points.end(); ++point_it)
	{
		glm::vec2 point = *point_it + a_Cursor;

		polyline.push_back(new p2t::Point((double)point.x, (double)point.y));
	}

	return polyline;
}*/

TextMesh CreateMesh(ExLibris::FontFace* a_Face, const std::wstring& a_Text)
{
	glm::vec2 cursor_offset;

	TextMesh text_mesh;

	std::vector<ExLibris::Glyph*> glyphs;

	for (std::wstring::const_iterator char_it = a_Text.begin(); char_it != a_Text.end(); ++char_it)
	{
		unsigned int character = (unsigned int)*char_it;

		ExLibris::Glyph* glyph = a_Face->FindGlyph(character);
		if (glyph != nullptr)
		{
			glyphs.push_back(glyph);
		}
	}

	text_mesh.glyphs.reserve(glyphs.size());

	std::vector<ExLibris::Glyph*>::iterator glyph_next_it = glyphs.begin() + 1;

	for (std::vector<ExLibris::Glyph*>::iterator glyph_it = glyphs.begin(); glyph_it != glyphs.end(); ++glyph_it)
	{
		ExLibris::Glyph* glyph = *glyph_it;

		if (glyph->mesh != nullptr)
		{
			TextGlyphMesh* glyph_mesh = new TextGlyphMesh;
			glyph_mesh->line_offset = glm::vec2(0.0f, glyph->metrics->offset.y);
			glyph_mesh->glyph_offset = cursor_offset;

			glyph_mesh->vertex_count = glyph->mesh->vertex_count;

			glGenBuffers(1, &glyph_mesh->vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, glyph_mesh->vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, glyph_mesh->vertex_count * sizeof(glm::vec2), glyph->mesh->positions, GL_STATIC_DRAW);

			text_mesh.glyphs.push_back(glyph_mesh);
		}

		cursor_offset.x += glyph->metrics->advance;

		if (glyph_next_it != glyphs.end())
		{
			glyph_next_it++;
		}
	}

	text_mesh.dimensions.x = cursor_offset.x;
	text_mesh.dimensions.y = a_Face->GetLineHeight();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return text_mesh;
}

class MeshOpenGL
	: public ExLibris::IMeshVisitor
{

public:

	MeshOpenGL()
		: m_Buffer(0)
		, m_VertexData(nullptr)
		, m_VertexCount(0)
		, m_VertexFilled(0)
	{
		glGenBuffers(1, &m_Buffer);
	}

	~MeshOpenGL()
	{
		if (m_VertexData != nullptr)
		{
			delete [] m_VertexData;
			m_VertexData = nullptr;
		}

		glDeleteBuffers(1, &m_Buffer);
	}

	ExLibris::TriangleOrientation GetOrientation() const
	{
		return ExLibris::eTriangleOrientation_CounterClockWise;
	}

	void VisitBuilderMeshBegin(unsigned int a_VertexCount)
	{
		m_VertexFilled = 0;

		if (a_VertexCount > m_VertexCount)
		{
			m_VertexCount = a_VertexCount;

			if (m_VertexData != nullptr)
			{
				delete [] m_VertexData;
			}
			m_VertexData = new glm::vec2[m_VertexCount];
		}
	}

	void VisitBuilderTriangle(const glm::vec2& a_A, const glm::vec2& a_B, const glm::vec2& a_C)
	{
		m_VertexData[m_VertexFilled++] = a_A;
		m_VertexData[m_VertexFilled++] = a_B;
		m_VertexData[m_VertexFilled++] = a_C;
	}

	void VisitBuilderMeshEnd()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
		glBufferData(GL_ARRAY_BUFFER, m_VertexFilled * sizeof(glm::vec2), m_VertexData, GL_STATIC_DRAW);
	}

	void Render()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
		glDrawArrays(GL_TRIANGLES, 0, m_VertexFilled);
	}

private:

	GLuint m_Buffer;
	glm::vec2* m_VertexData;
	size_t m_VertexCount;
	size_t m_VertexFilled;

};


//#define SKIP_FONT

int main(int argc, const char** argv)
{
	GLFWwindow* window;

	glfwSetErrorCallback(OnGlfwError);

	if (glfwInit() == 0)
	{
		exit(EXIT_FAILURE);
	}

	ExLibris::FontFace* face_size24 = nullptr;

#ifndef SKIP_FONT

	if (g_LoadFont)
	{
		ExLibris::FontLoaderFreetype loader;

		float load_time_start = (float)glfwGetTime();

		ExLibris::IFont* font = loader.LoadFont(g_FontPath);

		float load_time_font = (float)glfwGetTime();
		std::cout << "Loading font: " << (load_time_font - load_time_start) * 1000.0f << " ms." << std::endl;
		load_time_start = load_time_font;

		ExLibris::FontFace* face_size24 = font->CreateFace(g_FontSize);

		float load_time_face = (float)glfwGetTime();
		std::cout << "Creating face: " << (load_time_face - load_time_start) * 1000.0f << " ms." << std::endl;
		load_time_start = load_time_face;
	}

#endif

	window = glfwCreateWindow(640, 480, "ExLibris", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, OnKey);
	glfwSetMouseButtonCallback(window, OnMouseButton);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW." << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_2D);

	g_ShaderLoader = new Framework::ShaderLoader();
	LoadShaders();

#ifndef SKIP_FONT

	TextOutline outline;
	TextMesh mesh;

	ExLibris::Glyph* glyph_a = nullptr;
	ExLibris::LineShape shape_a;
	ExLibris::TriangleList* list_a = nullptr;
	GLuint buffer_a = 0;

	if (g_LoadFont)
	{
		glyph_a = face_size24->FindGlyph((unsigned int)'$');

		for (std::vector<ExLibris::Polygon>::iterator contour_it = glyph_a->outline->contours.begin(); contour_it != glyph_a->outline->contours.end(); ++contour_it)
		{
			shape_a.AddPolygon(*contour_it);
		}

		list_a = shape_a.Triangulate(5.0f);

		glGenBuffers(1, &buffer_a);
		glBindBuffer(GL_ARRAY_BUFFER, buffer_a);
		glBufferData(GL_ARRAY_BUFFER, list_a->vertex_filled * sizeof(glm::vec2), list_a->positions, GL_STATIC_DRAW);

		outline = CreateTextOutline(face_size24, g_Text);

		mesh = CreateMesh(face_size24, g_Text);
	}

#endif

	float time = 0.0f;

	/*ExLibris::LineShape shape;

	ExLibris::Polygon p;
	p += glm::vec2(0.0f, 100.0f);
	p += glm::vec2(10.0f, 0.0f);
	p += glm::vec2(20.0f, 100.0f);

	shape.AddPolygon(p);

	ExLibris::TriangleList* triangles_test = shape.Triangulate(20.0f);*/

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);

	MeshOpenGL* mesh_outline = new MeshOpenGL();

	float timer = 0.0f;

	unsigned long time_start = GetTickCount();
	float physics_time = 0.0f;
	float physics_frame = 1000.0f / 60.0f;

	do 
	{
		glm::dvec2 mouse_position;
		glfwGetCursorPos(window, &mouse_position.x, &mouse_position.y);

		unsigned long time_current = GetTickCount();
		float time_delta = (float)(time_current - time_start);
		time_start = time_current;

		physics_time += time_delta;

		// update

		while (physics_time > physics_frame)
		{
			timer += 0.05f;

			physics_time -= physics_frame;
		}

		// render

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;

		char window_title[256] = { 0 };
		sprintf(window_title, "ExLibris - (%i, %i)", (int)mouse_position.x, (int)mouse_position.y);
		glfwSetWindowTitle(window, window_title);

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4x4 projection = glm::ortho<float>(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
		
		glm::mat4x4 modelview;
		modelview = glm::translate(modelview, g_CameraPosition);
		modelview = glm::scale(modelview, glm::vec3(g_CameraZoom, g_CameraZoom, 1.0f));

		glm::mat4x4 mvp = projection * modelview;

		if (g_LoadFont)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(glm::value_ptr(projection));

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(modelview));

			glColor4fv(glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

			glBindBuffer(GL_ARRAY_BUFFER, buffer_a);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), 0);

			glDrawArrays(GL_TRIANGLES, 0, list_a->vertex_filled);

			glDisableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

/*#ifndef SKIP_FONT

		glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);

		glUseProgram(g_ShaderProgram->GetHandle());
		glUniformMatrix4fv(g_ShaderProgram->GetUniform("matModelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniform4fv(g_ShaderProgram->GetUniform("uniColor"), 1, glm::value_ptr(color));
		glUniform1f(g_ShaderProgram->GetUniform("uniTime"), timer);
		glUniform2fv(g_ShaderProgram->GetUniform("uniTextDimensions"), 1, glm::value_ptr(mesh.dimensions));

		GLint attribute_position = g_ShaderProgram->GetAttribute("attrPosition");
		glEnableVertexAttribArray(attribute_position);

		for (std::vector<TextGlyphMesh*>::iterator mesh_it = mesh.glyphs.begin(); mesh_it != mesh.glyphs.end(); ++mesh_it)
		{
			TextGlyphMesh* glyph_mesh = *mesh_it;

			glUniform2fv(g_ShaderProgram->GetUniform("uniTextOffset"), 1, glm::value_ptr(glyph_mesh->line_offset));
			glUniform2fv(g_ShaderProgram->GetUniform("uniGlyphOffset"), 1, glm::value_ptr(glyph_mesh->glyph_offset));
			glBindBuffer(GL_ARRAY_BUFFER, glyph_mesh->vertex_buffer);

			glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

			glDrawArrays(GL_TRIANGLES, 0, glyph_mesh->vertex_count);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);

#endif*/

		g_MousePath += glm::vec2((float)mouse_position.x, (float)mouse_position.y);

		ExLibris::LineShape shape;
		shape.AddPolygon(g_MousePath);

		ExLibris::LineMeshOptions options;
		options.thickness = 20.0f;
		options.quality = ExLibris::LineMeshOptions::eQuality_Fast;

		ExLibris::MeshBuilder* builder = shape.BuildMesh(options);

		g_MousePath.positions.pop_back();

		if (builder != nullptr && builder->GetVertexCount() > 0)
		{
			builder->Accept(*mesh_outline);

			Framework::ShaderProgram* outline_program = nullptr;
			glm::vec4 outline_color;
			
			if (g_DrawLines)
			{
				outline_program = g_ProgramOutlineLines;
				outline_color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else
			{
				outline_program = g_ProgramOutlineTriangles;
				outline_color = glm::vec4(0.0f, 0.0f, 0.75f, 1.0f);
			}

			glUseProgram(outline_program->GetHandle());
			glUniformMatrix4fv(outline_program->GetUniform("matModelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));
			glUniform4fv(outline_program->GetUniform("uniColor"), 1, glm::value_ptr(outline_color));

			GLint attribute_position = outline_program->GetAttribute("attrPosition");
			glEnableVertexAttribArray(attribute_position);

			glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

			mesh_outline->Render();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glUseProgram(0);

			delete builder;
		}

		/*glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), 0);

		glDrawArrays(GL_TRIANGLES, 0, 12);

		glBindBuffer(GL_ARRAY_BUFFER, 0);*/

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, text_texture);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

		glVertexPointer(2, GL_FLOAT, sizeof(GlyphVertex), (void*)GlyphVertex::eOffset_Position);
		glTexCoordPointer(2, GL_FLOAT, sizeof(GlyphVertex), (void*)GlyphVertex::eOffset_TextureCoordinate);

		glDrawElements(GL_TRIANGLES, element_total, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	while (glfwWindowShouldClose(window) == 0);

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}