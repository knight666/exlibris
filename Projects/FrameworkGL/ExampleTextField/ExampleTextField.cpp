// STL

#include <iostream>
#include <map>
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

// ExLibris

#include <FontFace.h>
#include <FontFreetype.h>
#include <FontLoaderFreetype.h>
#include <Library.h>
#include <TextLayout.h>

namespace exl = ExLibris;

class TextField
	: public exl::ITextLayoutVisitor
{

private:

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

public:

	TextField(fw::ShaderProgram* a_Program)
		: m_Layout(nullptr)
		, m_Texture(0)
		, m_TextureWidth(0)
		, m_TexturePitch(0)
		, m_TextureHeight(0)
		, m_TextureData(nullptr)
		, m_TexturePadding(8, 8)
		, m_CursorVisible(true)
		, m_CursorTime(0.0f)
		, m_BufferVertices(0)
		, m_BufferElements(0)
		, m_BufferAttributes(0)
		, m_HelperLayout(nullptr)
		, m_HelperLayoutVisible(true)
		, m_HelperLines(nullptr)
		, m_HelperLinesVisible(true)
		, m_HelperGlyphs(nullptr)
		, m_HelperGlyphsVisible(true)
		, m_HelperBitmaps(nullptr)
		, m_HelperBitmapsVisible(true)
	{
		m_Layout = new exl::TextLayout;

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenBuffers(1, &m_BufferVertices);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferVertices);

			GlyphVertex vertex_data[4] = {
				{
					glm::vec2(0.0f, 0.0f),
					glm::vec2(0.0f, 0.0f)
				},
				{
					glm::vec2(1.0f, 0.0f),
					glm::vec2(1.0f, 0.0f)
				},
				{
					glm::vec2(0.0f, 1.0f),
					glm::vec2(0.0f, 1.0f)
				},
				{
					glm::vec2(1.0f, 1.0f),
					glm::vec2(1.0f, 1.0f)
				},
			};

			glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GlyphVertex), vertex_data, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_BufferElements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferElements);

			GLuint element_data[6] = {
				1, 0, 2,
				1, 2, 3
			};

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), element_data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &m_BufferAttributes);

		SetShaderProgram(a_Program);

		m_HelperLayout = new fw::DebugHelper;
		m_HelperLines = new fw::DebugHelper;
		m_HelperGlyphs = new fw::DebugHelper;
		m_HelperBitmaps = new fw::DebugHelper;
	}

	~TextField()
	{
		delete m_Layout;

		glDeleteTextures(1, &m_Texture);
		glDeleteBuffers(1, &m_BufferVertices);
		glDeleteBuffers(1, &m_BufferElements);
		glDeleteVertexArrays(1, &m_BufferAttributes);

		delete m_HelperLayout;
		delete m_HelperLines;
		delete m_HelperGlyphs;
		delete m_HelperBitmaps;
	}

	void ToggleLayoutOutlineVisible()
	{
		m_HelperLayoutVisible = !m_HelperLayoutVisible;
	}

	void ToggleLinesOutlineVisible()
	{
		m_HelperLinesVisible = !m_HelperLinesVisible;
	}

	void ToggleGlyphsOutlineVisible()
	{
		m_HelperGlyphsVisible = !m_HelperGlyphsVisible;
	}

	void ToggleBitmapsOutlineVisible()
	{
		m_HelperBitmapsVisible = !m_HelperBitmapsVisible;
	}

	void SetShaderProgram(fw::ShaderProgram* a_Program)
	{
		m_Program = a_Program;

		glBindVertexArray(m_BufferAttributes);

			glBindBuffer(GL_ARRAY_BUFFER, m_BufferVertices);

			GLint attribute_position = m_Program->FindAttribute("attrPosition");
			glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (const GLvoid*)GlyphVertex::eOffset_Position);
			glEnableVertexAttribArray(attribute_position);

			GLint attribute_texturecoordinate = m_Program->FindAttribute("attrTextureCoordinate0");
			glVertexAttribPointer(attribute_texturecoordinate, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (const GLvoid*)GlyphVertex::eOffset_TextureCoordinate);
			glEnableVertexAttribArray(attribute_texturecoordinate);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferElements);

		glBindVertexArray(0);
	}

	void SetFont(exl::FontFace* a_Font)
	{
		m_Font = a_Font;
		m_Layout->SetFontFace(m_Font);

		m_RenderCorrection.y = -m_Font->GetAscender();

		m_Layout->Accept(*this);
	}

	void SetPosition(const glm::vec2& a_Position)
	{
		m_Position = a_Position;
	}

	void AddCharacter(unsigned int a_Character)
	{
		m_Text.push_back((char)a_Character);

		m_Layout->SetText(m_Text);
		m_Layout->Accept(*this);
	}

	void PopCharacter()
	{
		if (m_Text.size() > 0)
		{
			m_Text.pop_back();

			m_Layout->SetText(m_Text);
			m_Layout->Accept(*this);
		}
	}

	void Update(float a_DeltaTime)
	{
		m_CursorTime += a_DeltaTime;

		while (m_CursorTime > 50.0f)
		{
			m_CursorVisible = !m_CursorVisible;

			m_CursorTime -= 50.0f;
		}
	}

	void Render(int a_Width, int a_Height)
	{
		glm::vec2 screen_position = m_Position + glm::vec2(-m_TexturePadding) + glm::vec2(0.0f, -m_Font->GetAscender()) + m_LineCorrection;

		glm::mat4x4 modelview;
		modelview = glm::translate(modelview, glm::vec3(screen_position.x, screen_position.y, 0.0f));
		modelview = glm::scale(modelview, glm::vec3((float)m_TextureWidth, (float)m_TextureHeight, 1.0f));

		glm::mat4x4 projection = glm::ortho<float>(
			0.0f, (float)a_Width,
			(float)a_Height, 0.0f,
			-1.0f, 1.0f
		);

		glm::mat4x4 mvp = projection * modelview;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// render text
			
		glUniformMatrix4fv(m_Program->FindUniform("matModelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));

		glUniform2fv(m_Program->FindUniform("uniTextureDimensions"), 1, glm::value_ptr(m_TextureDimensions));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glUniform1i(m_Program->FindUniform("texTexture0"), 0);

		glBindVertexArray(m_BufferAttributes);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);
		glUseProgram(0);

		// render cursor

		if (m_CursorVisible)
		{
			glDisable(GL_DEPTH_TEST);

			glm::vec2 cursor_position = screen_position + m_CursorPosition + glm::vec2(-m_TexturePadding);

			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(glm::value_ptr(projection));

			glm::mat4x4 modelview_cursor;
			modelview_cursor = glm::translate(modelview_cursor, glm::vec3(cursor_position.x, cursor_position.y, 0.0f));
			modelview_cursor = glm::scale(modelview_cursor, glm::vec3(1.0f, m_Font->GetLineHeight(), 1.0f));

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(modelview_cursor));

			glBindVertexArray(m_BufferAttributes);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glEnable(GL_DEPTH_TEST);
		}

		if (m_HelperLayoutVisible)
		{
			m_HelperLayout->Render(a_Width, a_Height);
		}
		
		if (m_HelperLinesVisible)
		{
			m_HelperLines->Render(a_Width, a_Height);
		}
		
		if (m_HelperGlyphsVisible)
		{
			m_HelperGlyphs->Render(a_Width, a_Height);
		}
		
		if (m_HelperBitmapsVisible)
		{
			m_HelperBitmaps->Render(a_Width, a_Height);
		}
	}

private:

	void VisitTextBegin(const exl::FontFace* a_Face, const glm::vec2& a_Dimensions, const ExLibris::BoundingBox& a_BoundingBox)
	{
		// texture must be padded in order to support effects like glow and shadows

		glm::vec2 dimensions = a_BoundingBox.GetDimensions();

		m_TextureWidth = (unsigned int)dimensions.x + (m_TexturePadding.x * 2);
		m_TexturePitch = m_TextureWidth * 4;
		m_TextureHeight = (unsigned int)dimensions.y + (m_TexturePadding.y * 2);

		m_TextureDimensions.x = (float)m_TextureWidth;
		m_TextureDimensions.y = (float)m_TextureHeight;

		m_TextureData = new unsigned char[m_TexturePitch * m_TextureHeight];

		unsigned int clear_color = 0x00000000;

		for (unsigned int y = 0; y < m_TextureHeight; ++y)
		{
			unsigned int* dst = (unsigned int*)(m_TextureData + y * m_TexturePitch);

			for (unsigned int x = 0; x < m_TextureWidth; ++x)
			{
				*dst++ = clear_color;
			}
		}

		m_RenderCorrection.x = (float)(-m_TexturePadding.x);
		m_RenderCorrection.y = (float)(-m_TexturePadding.y) - a_Face->GetAscender();

		m_LineCorrection = a_BoundingBox.GetMinimum();

		m_TextureCorrection = glm::vec2(0.0f, m_Font->GetDescender() + m_Font->GetAscender()) - a_BoundingBox.GetMinimum() + glm::vec2(m_TexturePadding);

		m_HelperLayout->Clear();
		m_HelperLayout->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		m_HelperLines->Clear();
		m_HelperLines->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		m_HelperGlyphs->Clear();
		m_HelperGlyphs->SetColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

		m_HelperBitmaps->Clear();
		m_HelperBitmaps->SetColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

		exl::BoundingBox box = a_BoundingBox.GetTranslated(m_Position + glm::vec2(0.0f, -a_Face->GetAscender()));

		m_HelperLayout->AddBox(box);
	}

	void VisitTextLineBegin(size_t a_GlyphCount, const glm::vec2& a_Offset, float a_Width, const exl::BoundingBox& a_BoundingBox)
	{
		//m_LineOffset = a_Offset;
		//m_LineOffset.y += m_Font->GetDescender();

		m_LineOffset.x = 0.0f;
		m_LineOffset.y = m_Font->GetDescender();

		m_CursorPosition = a_Offset;

		exl::BoundingBox box = a_BoundingBox;
		box.Translate(m_Position + m_LineOffset);

		m_HelperLines->AddBox(box);
	}

	void VisitTextCharacter(const exl::Glyph* a_Glyph, float a_X, float a_Advance, const exl::BoundingBox& a_BoundingBox)
	{
		exl::GlyphMetrics* metrics = a_Glyph->metrics;
		exl::GlyphBitmap* bitmap = a_Glyph->bitmap;

		m_CursorPosition.x = a_X + a_Advance;

		glm::vec2 texture_position = a_BoundingBox.GetMinimum() + m_TextureCorrection;

		exl::BoundingBox box = a_BoundingBox;
		box.Translate(m_Position + m_LineOffset);

		m_HelperGlyphs->AddBox(box);

		unsigned char* dst = m_TextureData + ((unsigned int)texture_position.y * m_TexturePitch) + ((unsigned int)texture_position.x * 4);
		unsigned char* dst_end = m_TextureData + m_TexturePitch * m_TextureHeight;

		unsigned int src_pitch = bitmap->width * 4;
		unsigned char* src = bitmap->data;

		for (unsigned int y = 0; y < bitmap->height; ++y)
		{
			if (dst >= m_TextureData && dst + src_pitch < dst_end)
			{
				unsigned int* dst_line = (unsigned int*)dst;
				unsigned int* src_line = (unsigned int*)src;

				for (unsigned int x = 0; x < bitmap->width; ++x)
				{
					unsigned int src_a = (*src_line & 0x000000FF);
					unsigned int src_r = (*src_line & 0x0000FF00) >> 8;
					unsigned int src_g = (*src_line & 0x00FF0000) >> 16;
					unsigned int src_b = (*src_line & 0xFF000000) >> 24;

					unsigned int dst_a = (*dst_line & 0x000000FF);
					unsigned int dst_r = (*dst_line & 0x0000FF00) >> 8;
					unsigned int dst_g = (*dst_line & 0x00FF0000) >> 16;
					unsigned int dst_b = (*dst_line & 0xFF000000) >> 24;

					unsigned int rgba =
						((((src_a * src_a) + (dst_a * (255 - src_a))) >> 8) & 0x000000FF) |
						((((src_r * src_a) + (dst_r * (255 - src_a))) >> 8) & 0x000000FF) << 8 |
						((((src_g * src_a) + (dst_g * (255 - src_a))) >> 8) & 0x000000FF) << 16 |
						((((src_b * src_a) + (dst_b * (255 - src_a))) >> 8) & 0x000000FF) << 24;

					*dst_line = rgba;

					++dst_line;
					++src_line;
				}
			
				dst += m_TexturePitch;
				src += src_pitch;
			}
		}
	}

	void VisitTextWhitespace(unsigned int a_Identifier, float a_X, float a_Advance, const exl::BoundingBox& a_BoundingBox)
	{
		m_CursorPosition.x = a_X + a_Advance;
	}

	void VisitTextLineEnd()
	{
	}

	void VisitTextEnd()
	{
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA,
			m_TextureWidth, m_TextureHeight,
			0,
			GL_BGRA, GL_UNSIGNED_BYTE, m_TextureData
		);

		delete [] m_TextureData;
		m_TextureData = nullptr;

		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:

	fw::DebugHelper* m_HelperLayout;
	bool m_HelperLayoutVisible;
	fw::DebugHelper* m_HelperLines;
	bool m_HelperLinesVisible;
	fw::DebugHelper* m_HelperGlyphs;
	bool m_HelperGlyphsVisible;
	fw::DebugHelper* m_HelperBitmaps;
	bool m_HelperBitmapsVisible;

	glm::vec2 m_Position;
	exl::FontFace* m_Font;
	exl::TextLayout* m_Layout;
	std::string m_Text;

	glm::vec2 m_LineOffset;
	glm::vec2 m_LineCorrection;
	glm::vec2 m_RenderCorrection;
	glm::vec2 m_TextureCorrection;

	glm::vec2 m_CursorPosition;
	bool m_CursorVisible;
	float m_CursorTime;

	GLuint m_Texture;
	unsigned int m_TextureWidth;
	unsigned int m_TexturePitch;
	unsigned int m_TextureHeight;
	glm::vec2 m_TextureDimensions;
	unsigned char* m_TextureData;
	glm::ivec2 m_TexturePadding;

	fw::ShaderProgram* m_Program;
	GLuint m_BufferVertices;
	GLuint m_BufferElements;
	GLuint m_BufferAttributes;

}; // class TextField

class ExampleTextField
	: public fw::Application
{

public:

	ExampleTextField(int a_ArgumentCount, const char** a_Arguments)
		: fw::Application(a_ArgumentCount, a_Arguments)
		, m_Library(nullptr)
		, m_TextField(nullptr)
		, m_FontFace(nullptr)
		, m_ProgramEffects(nullptr)
		, m_UseShadow(false)
		, m_UseGlow(false)
	{
	}

	~ExampleTextField()
	{
	}

	bool ParseCommandLine(int a_ArgumentCount, const char** a_Arguments)
	{
		return true;
	}
	
	bool Initialize()
	{
		_LoadShaders();

		try
		{
			m_DebugHelper = new fw::DebugHelper;
		}
		catch (std::exception& e)
		{
			MessageBoxA(0, e.what(), "Error while creating TextHelper", MB_OK);
			return false;
		}

		m_Library = new exl::Library;
		m_Library->AddLoader(new exl::FontLoaderFreetype(m_Library));

		m_FaceOptions.size = 60.0f;

		//m_Font = m_Library->LoadFont("Fonts/Roboto/Roboto-Regular.ttf");
		m_Font = m_Library->LoadFont("Fonts/Mathilde/mathilde.otf");

		m_FontFace = m_Font->CreateFace(m_FaceOptions);

		m_TextField = new TextField(m_ProgramEffects);
		m_TextField->SetFont(m_FontFace);
		m_TextField->SetPosition(glm::vec2(100.0f, 100.0f));

		return true;
	}

	void Update(float a_DeltaTime)
	{
		m_TextField->Update(a_DeltaTime);
	}

	void Render()
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize(GetWindow(), &width, &height);
		ratio = width / (float) height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_ProgramEffects->GetHandle());
		glUniform1i(m_ProgramEffects->FindUniform("uniUseShadow"), m_UseShadow ? GL_TRUE : GL_FALSE);
		glUniform1i(m_ProgramEffects->FindUniform("uniUseGlow"), m_UseGlow ? GL_TRUE : GL_FALSE);

		m_TextField->Render(width, height);

		glm::dvec2 mouse_position;
		glfwGetCursorPos(GetWindow(), &mouse_position.x, &mouse_position.y);

		m_DebugHelper->Clear();

		char mouse_text[256] = { 0 };
		sprintf(mouse_text, "Mouse position: (%.2f, %.2f)", mouse_position.x, mouse_position.y);
		glfwSetWindowTitle(GetWindow(), mouse_text);
		//m_DebugHelper->AddText(mouse_text, glm::vec2(10.0f, 10.0f));

		m_DebugHelper->Render(width, height);
	}

	void Destroy()
	{
		delete m_DebugHelper;

		if (m_TextField != nullptr)
		{
			delete m_TextField;
		}

		delete m_Library;

		if (m_ProgramEffects != nullptr)
		{
			delete m_ProgramEffects;
		}
	}

private:

	void OnCharacter(unsigned int a_Character)
	{
		m_TextField->AddCharacter(a_Character);
	}

	void OnKeyReleased(int a_Key, int a_ScanCode, int a_Modifiers)
	{
		if (a_Modifiers & GLFW_MOD_CONTROL)
		{
			switch (a_Key)
			{

			case GLFW_KEY_G:
				{
					m_UseGlow = !m_UseGlow;

				} break;

			case GLFW_KEY_S:
				{
					m_UseShadow = !m_UseShadow;

				} break;


			case GLFW_KEY_1:
				{
					m_TextField->ToggleLayoutOutlineVisible();

				} break;

			case GLFW_KEY_2:
				{
					m_TextField->ToggleLinesOutlineVisible();

				} break;

			case GLFW_KEY_3:
				{
					m_TextField->ToggleGlyphsOutlineVisible();

				} break;

			case GLFW_KEY_4:
				{
					m_TextField->ToggleBitmapsOutlineVisible();

				} break;

			}
		}

		switch (a_Key)
		{

		case GLFW_KEY_F5:
			{
				_LoadShaders();

				m_TextField->SetShaderProgram(m_ProgramEffects);

			} break;

		case GLFW_KEY_BACKSPACE:
			{
				m_TextField->PopCharacter();

			} break;

		case GLFW_KEY_ENTER:
			{
				m_TextField->AddCharacter((unsigned int)'\n');

			} break;

		case GLFW_KEY_UP:
			{
				m_FaceOptions.size -= 1.0f;

				m_FontFace = m_Font->CreateFace(m_FaceOptions);
				m_TextField->SetFont(m_FontFace);

			} break;

		case GLFW_KEY_DOWN:
			{
				m_FaceOptions.size += 1.0f;

				m_FontFace = m_Font->CreateFace(m_FaceOptions);
				m_TextField->SetFont(m_FontFace);

			} break;

		}
	}

private:

	void _LoadShaders()
	{
		try
		{
			if (m_ProgramEffects == nullptr)
			{
				m_ProgramEffects = new fw::ShaderProgram();
			}

			m_ProgramEffects->LoadSourceFromFile(GL_VERTEX_SHADER, "Shaders/TextEffects.vert");
			m_ProgramEffects->LoadSourceFromFile(GL_FRAGMENT_SHADER, "Shaders/TextEffects.frag");
			m_ProgramEffects->Link();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

private:

	exl::Library* m_Library;
	exl::IFont* m_Font;
	exl::FaceOptions m_FaceOptions;
	exl::FontFace* m_FontFace;
	fw::DebugHelper* m_DebugHelper;

	TextField* m_TextField;

	fw::ShaderProgram* m_ProgramEffects;

	bool m_UseGlow;
	bool m_UseShadow;

}; // class ExampleTextField

int main(int argc, const char** argv)
{
	ExampleTextField application(argc, argv);
	return application.Run();
}