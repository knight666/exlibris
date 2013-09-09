// STL

#include <iostream>
#include <string>
#include <map>
#include <vector>

// Freetype

#include <freetype/ftbbox.h>
#include <freetype/ftoutln.h>

// GLM

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OpenGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// ExLibris

#include <FaceMetrics.h>
#include <FontFace.h>
#include <FontLoaderFreetype.h>
#include <Glyph.h>

static void OnGlfwError(int error, const char* description)
{
	std::cerr << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
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
			for (std::vector<ExLibris::GlyphContour*>::iterator contour_it = glyph->outline->contours.begin(); contour_it != glyph->outline->contours.end(); ++contour_it)
			{
				ExLibris::GlyphContour* contour = *contour_it;

				outline.vertex_count += contour->points.size();
			}
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

	for (std::vector<ExLibris::Glyph*>::iterator glyph_it = glyphs.begin(); glyph_it != glyphs.end(); ++glyph_it)
	{
		ExLibris::Glyph* glyph = *glyph_it;

		if (glyph->outline != nullptr)
		{
			glm::vec2 position_local = cursor_offset;
			position_local.y += glyph->metrics->offset.y;

			for (std::vector<ExLibris::GlyphContour*>::iterator contour_it = glyph->outline->contours.begin(); contour_it != glyph->outline->contours.end(); ++contour_it)
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
			}
		}
		
		cursor_offset.x += glyph->metrics->advance;
	}

	glGenBuffers(1, &outline.vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, outline.vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, outline.vertex_count * sizeof(glm::vec2), position_data, GL_STATIC_DRAW);

	delete [] position_data;

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return outline;
}

int main(int argc, const char** argv)
{
	ExLibris::FontLoaderFreetype loader;
	ExLibris::IFont* font = loader.LoadFont("Fonts/Mathilde/mathilde.otf");
	//ExLibris::IFont* font = loader.LoadFont("Fonts/Roboto/Roboto-Regular.ttf");
	ExLibris::FontFace* face_size24 = font->CreateFace(12.0f);

	ExLibris::Glyph* glyph = face_size24->FindGlyph((unsigned int)'Q');

	GLFWwindow* window;

	glfwSetErrorCallback(OnGlfwError);

	if (glfwInit() == 0)
	{
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(640, 480, "ExLibris", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW." << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_2D);

	std::wstring text = L"Pa's wijze lynx bezag vroom het fikse aquaduct";
	TextOutline outline = CreateTextOutline(face_size24, text);

	//GLuint text_texture = CreateTexture(font_glyphs, (unsigned int)font_face_height, text);

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	unsigned int vertex_total = 4;
	GlyphVertex* vertex_data = new GlyphVertex[vertex_total];

	GLuint element_buffer;
	glGenBuffers(1, &element_buffer);
	unsigned int element_total = 6;
	GLuint* element_data = new GLuint[element_total];

	vertex_data[0].position.x = 0.0f;
	vertex_data[0].position.y = 0.0f;
	vertex_data[0].texture_coordinate.x = 0.0f;
	vertex_data[0].texture_coordinate.y = 1.0f;

	vertex_data[1].position.x = 2.0f;
	vertex_data[1].position.y = 0.0f;
	vertex_data[1].texture_coordinate.x = 1.0f;
	vertex_data[1].texture_coordinate.y = 1.0f;

	vertex_data[2].position.x = 0.0f;
	vertex_data[2].position.y = 1.0f;
	vertex_data[2].texture_coordinate.x = 0.0f;
	vertex_data[2].texture_coordinate.y = 0.0f;

	vertex_data[3].position.x = 2.0f;
	vertex_data[3].position.y = 1.0f;
	vertex_data[3].texture_coordinate.x = 1.0f;
	vertex_data[3].texture_coordinate.y = 0.0f;

	element_data[0] = 1;
	element_data[1] = 0;
	element_data[2] = 2;

	element_data[3] = 1;
	element_data[4] = 2;
	element_data[5] = 3;

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_total * sizeof(GlyphVertex), vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_total * sizeof(GLuint), element_data, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	while (glfwWindowShouldClose(window) == 0)
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glm::mat4x4 projection = glm::ortho<float>(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
		glLoadMatrixf(glm::value_ptr(projection));

		glMatrixMode(GL_MODELVIEW);
		glm::mat4x4 modelview;
		modelview = glm::scale(modelview, glm::vec3(5.0f, 5.0f, 5.0f));
		glLoadMatrixf(glm::value_ptr(modelview));

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glBindBuffer(GL_ARRAY_BUFFER, outline.vertex_buffer);

		glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), 0);
		
		for (std::vector<TextGlyphOutline>::iterator contour_it = outline.contours.begin(); contour_it != outline.contours.end(); ++contour_it)
		{
			TextGlyphOutline& contour = *contour_it;

			glDrawArrays(GL_LINE_LOOP, contour.start, contour.count);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

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

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}