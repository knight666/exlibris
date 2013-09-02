#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <iostream>
#include <map>

#include "FontLoaderFreetype.h"

static void OnGlfwError(int error, const char* description)
{
	std::cerr << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

struct GlyphMetrics
{
	unsigned int codepoint;
	glm::vec2 offset;
	float advance;
};

int main(int argc, const char** argv)
{
	ExLibris::FontLoaderFreetype loader;
	FT_Face font_face = loader.LoadFontFace("Fonts/Roboto/Roboto-Regular.ttf");

	FT_Error errors = 0;

	errors = FT_Set_Char_Size(font_face, 0, 24 << 6, 0, 96);

	std::map<unsigned int, GlyphMetrics*> font_metrics;

	FT_UInt codepoint = 0;
	FT_ULong glyph_index = FT_Get_First_Char(font_face, &codepoint);
	do
	{
		errors = FT_Load_Glyph(font_face, codepoint, FT_LOAD_DEFAULT);
		FT_Glyph_Metrics& glyph_metrics = font_face->glyph->metrics;

		GlyphMetrics* metrics = new GlyphMetrics;
		metrics->codepoint = (unsigned int)glyph_index;
		metrics->offset.x = (float)((glyph_metrics.horiBearingX) >> 6);
		metrics->offset.y = (float)((glyph_metrics.vertAdvance - glyph_metrics.horiBearingY) >> 6);
		metrics->advance = (float)((glyph_metrics.horiAdvance) >> 6);

		font_metrics.insert(std::make_pair(metrics->codepoint, metrics));

		glyph_index = FT_Get_Next_Char(font_face, glyph_index, &codepoint);
	}
	while (codepoint != 0);

	GLFWwindow* window;

	glfwSetErrorCallback(OnGlfwError);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(640, 480, "Simple example", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}