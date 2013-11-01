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
	}

	~TextField()
	{
		delete m_Layout;

		glDeleteTextures(1, &m_Texture);
		glDeleteBuffers(1, &m_BufferVertices);
		glDeleteBuffers(1, &m_BufferElements);
		glDeleteVertexArrays(1, &m_BufferAttributes);
	}

	void SetShaderProgram(fw::ShaderProgram* a_Program)
	{
		m_Program = a_Program;

		glBindVertexArray(m_BufferAttributes);

			glBindBuffer(GL_ARRAY_BUFFER, m_BufferVertices);

			GLint attribute_position = m_Program->GetAttribute("attrPosition");
			glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (const GLvoid*)GlyphVertex::eOffset_Position);
			glEnableVertexAttribArray(attribute_position);

			GLint attribute_texturecoordinate = m_Program->GetAttribute("attrTextureCoordinate0");
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

	void Render(const glm::vec2& a_Position, const glm::mat4x4& a_ProjectionMatrix)
	{
		glm::vec2 screen_position = a_Position + m_RenderCorrection + m_LineCorrection;

		glm::mat4x4 modelview;
		modelview = glm::translate(modelview, glm::vec3(screen_position.x, screen_position.y, 0.0f));
		modelview = glm::scale(modelview, glm::vec3((float)m_TextureWidth, (float)m_TextureHeight, 1.0f));

		glm::mat4x4 mvp = a_ProjectionMatrix * modelview;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// render text
			
		glUniformMatrix4fv(m_Program->GetUniform("matModelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));

		glUniform2fv(m_Program->GetUniform("uniTextureDimensions"), 1, glm::value_ptr(m_TextureDimensions));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glUniform1i(m_Program->GetUniform("texTexture0"), 0);

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

			glm::vec2 cursor_position = screen_position + m_CursorPosition + glm::vec2((float)m_TexturePadding.x, (float)m_TexturePadding.y);

			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(glm::value_ptr(a_ProjectionMatrix));

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
	}

private:

	void VisitTextBegin(const exl::FontFace* a_Face, const glm::vec2& a_Dimensions, const ExLibris::BoundingBox& a_BoundingBox)
	{
		// texture must be padded in order to support effects like glow and shadows

		m_TextureWidth = (unsigned int)a_Dimensions.x + (m_TexturePadding.x * 2);
		m_TexturePitch = m_TextureWidth * 4;
		m_TextureHeight = (unsigned int)a_Dimensions.y + (m_TexturePadding.y * 2);

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
	}

	void VisitTextLineBegin(size_t a_GlyphCount, const glm::vec2& a_Offset, float a_Width, const exl::BoundingBox& a_BoundingBox)
	{
		m_LineOffset = a_Offset;
		m_LineOffset.y += m_Font->GetDescender();

		m_CursorPosition = a_Offset;

		m_LineCorrection.x = 0.0f;
		m_LineCorrection.y = 0.0f;
	}

	void VisitTextCharacter(const exl::Glyph* a_Glyph, float a_X, float a_Advance)
	{
		exl::GlyphMetrics* metrics = a_Glyph->metrics;
		exl::GlyphBitmap* bitmap = a_Glyph->bitmap;

		m_CursorPosition.x = a_X + a_Advance;

		glm::vec2 offset = m_LineOffset + metrics->offset;
		offset.x += a_X;

		m_RenderCorrection.x = std::min(m_RenderCorrection.x, offset.x);

		if (offset.x < 0.0f)
		{
			m_LineCorrection.x = fabs(offset.x);
			offset.x = 0.0f;
		}

		unsigned char* dst = m_TextureData + (((unsigned int)offset.y + m_TexturePadding.y) * m_TexturePitch) + ((unsigned int)(offset.x + m_TexturePadding.x) * 4);
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

	void VisitTextWhitespace(unsigned int a_Identifier, float a_X, float a_Advance)
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

	exl::FontFace* m_Font;
	exl::TextLayout* m_Layout;
	std::string m_Text;

	glm::vec2 m_LineOffset;
	glm::vec2 m_LineCorrection;
	glm::vec2 m_RenderCorrection;

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
		m_ShaderLoader = new fw::ShaderLoader;
		
		_LoadShaders();

		m_Library = new exl::Library;
		m_Library->AddLoader(new exl::FontLoaderFreetype(m_Library));

		m_FaceOptions.size = 60.0f;

		//m_Font = m_Library->LoadFont("Fonts/Roboto/Roboto-Regular.ttf");
		m_Font = m_Library->LoadFont("Fonts/Mathilde/mathilde.otf");

		m_FontFace = m_Font->CreateFace(m_FaceOptions);

		m_TextField = new TextField(m_ProgramEffects);
		m_TextField->SetFont(m_FontFace);

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

		glm::mat4x4 projection = glm::ortho<float>(
			0.0f, (float)width,
			(float)height, 0.0f,
			-1.0f, 1.0f
		);

		glUseProgram(m_ProgramEffects->GetHandle());
		glUniform1i(m_ProgramEffects->GetUniform("uniUseShadow"), m_UseShadow ? GL_TRUE : GL_FALSE);
		glUniform1i(m_ProgramEffects->GetUniform("uniUseGlow"), m_UseGlow ? GL_TRUE : GL_FALSE);

		m_TextField->Render(glm::vec2(25.0f, 64.0f), projection);
	}

	void Destroy()
	{
		if (m_TextField != nullptr)
		{
			delete m_TextField;
		}

		delete m_Library;

		if (m_ProgramEffects != nullptr)
		{
			delete m_ProgramEffects;
		}

		delete m_ShaderLoader;
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
		if (m_ProgramEffects != nullptr)
		{
			delete m_ProgramEffects;
		}

		m_ProgramEffects = m_ShaderLoader->LoadProgram("TextEffects", "Shaders/TextEffects");
		m_ProgramEffects->Compile();
		m_ProgramEffects->Link();
	}

private:

	exl::Library* m_Library;
	exl::IFont* m_Font;
	exl::FaceOptions m_FaceOptions;
	exl::FontFace* m_FontFace;

	TextField* m_TextField;

	fw::ShaderLoader* m_ShaderLoader;
	fw::ShaderProgram* m_ProgramEffects;

	bool m_UseGlow;
	bool m_UseShadow;

}; // class ExampleTextField

int main(int argc, const char** argv)
{
	ExampleTextField application(argc, argv);
	return application.Run();
}