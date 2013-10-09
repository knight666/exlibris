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

	TextField()
		: m_Layout(nullptr)
		, m_Texture(0)
		, m_TextureWidth(0)
		, m_TexturePitch(0)
		, m_TextureHeight(0)
		, m_TextureData(nullptr)
	{
		m_Layout = new exl::TextLayout;

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);

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

		glGenBuffers(1, &m_BufferVertices);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferVertices);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GlyphVertex), vertex_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint element_data[6] = {
			1, 0, 2,
			1, 2, 3
		};

		glGenBuffers(1, &m_BufferElements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferElements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), element_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	~TextField()
	{
		delete m_Layout;

		glDeleteTextures(1, &m_Texture);
	}

	void SetFont(exl::FontFace* a_Font)
	{
		m_Font = a_Font;
		m_Layout->SetFontFace(m_Font);

		m_RenderCorrection.y = -m_Font->GetAscender();
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

	void Render(const glm::vec2& a_Position)
	{
		glm::vec2 screen_position = a_Position + m_RenderCorrection;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, m_BufferVertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferElements);

		glVertexPointer(2, GL_FLOAT, sizeof(GlyphVertex), (void*)GlyphVertex::eOffset_Position);
		glTexCoordPointer(2, GL_FLOAT, sizeof(GlyphVertex), (void*)GlyphVertex::eOffset_TextureCoordinate);

		// render text

		glm::mat4x4 modelview;
		modelview = glm::translate(modelview, glm::vec3(screen_position.x, screen_position.y, 0.0f));
		modelview = glm::scale(modelview, glm::vec3((float)m_TextureWidth, (float)m_TextureHeight, 1.0f));

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(modelview));

		glBindTexture(GL_TEXTURE_2D, m_Texture);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, 0);

		// render cursor

		glm::vec2 cursor_position = screen_position + m_CursorPosition;

		glm::mat4x4 modelview_cursor;
		modelview_cursor = glm::translate(modelview_cursor, glm::vec3(cursor_position.x, cursor_position.y, 0.0f));
		modelview_cursor = glm::scale(modelview_cursor, glm::vec3(1.0f, m_Layout->GetFontFace()->GetLineHeight(), 1.0f));

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(modelview_cursor));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisable(GL_BLEND);
	}

private:

	void VisitTextBegin(const exl::FontFace* a_Face, const glm::vec2& a_Dimensions)
	{
		m_TextureWidth = (unsigned int)a_Dimensions.x;
		m_TexturePitch = m_TextureWidth * 4;
		m_TextureHeight = (unsigned int)a_Dimensions.y;

		m_TextureData = new unsigned char[m_TexturePitch * m_TextureHeight];
		memset(m_TextureData, 0, m_TexturePitch * m_TextureHeight);

		m_RenderCorrection.x = 0.0f;
		m_RenderCorrection.y = -a_Face->GetAscender();
	}

	void VisitTextLineBegin(size_t a_GlyphCount, const glm::vec2& a_Offset, float a_Width)
	{
		m_LineOffset = a_Offset;
		m_LineOffset.y += m_Font->GetDescender();

		m_CursorPosition = a_Offset;
	}

	void VisitTextCharacter(const exl::Glyph* a_Glyph, float a_X, float a_Advance)
	{
		exl::GlyphMetrics* metrics = a_Glyph->metrics;
		exl::GlyphBitmap* bitmap = a_Glyph->bitmap;

		m_CursorPosition.x = a_X + a_Advance;

		glm::vec2 offset = m_LineOffset + metrics->offset;
		offset.x += a_X;

		m_RenderCorrection.x = std::min(m_RenderCorrection.x, offset.x);
		offset.x = std::max(offset.x, 0.0f);

		unsigned char* dst = m_TextureData + ((unsigned int)offset.y * m_TexturePitch) + (unsigned int)offset.x * 4;
		unsigned char* dst_end = m_TextureData + m_TexturePitch * m_TextureHeight;

		unsigned int src_pitch = bitmap->width * 4;
		unsigned char* src = bitmap->data;

		for (unsigned int y = 0; y < bitmap->height; ++y)
		{
			if (dst >= m_TextureData && dst < dst_end)
			{
				memcpy(dst, src, src_pitch);
			
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
	glm::vec2 m_RenderCorrection;
	glm::vec2 m_CursorPosition;

	GLuint m_Texture;
	unsigned int m_TextureWidth;
	unsigned int m_TexturePitch;
	unsigned int m_TextureHeight;
	unsigned char* m_TextureData;

	GLuint m_BufferVertices;
	GLuint m_BufferElements;

}; // class TextField

class ExampleTextField
	: public fw::Application
{

public:

	ExampleTextField(int a_ArgumentCount, const char** a_Arguments)
		: fw::Application(a_ArgumentCount, a_Arguments)
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
		m_Library = new exl::Library;
		m_FontLoader = new exl::FontLoaderFreetype(m_Library);

		m_Font = m_FontLoader->LoadFont("Fonts/Roboto/Roboto-Regular.ttf");
		m_FontFace = m_Font->CreateFace(24.0f);

		m_TextField = new TextField;
		m_TextField->SetFont(m_FontFace);

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

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(projection));

		m_TextField->Render(glm::vec2(25.0f, 32.0f));
	}

	void Destroy()
	{
	}

private:

	void OnCharacter(unsigned int a_Character)
	{
		m_TextField->AddCharacter(a_Character);
	}

	void OnKeyReleased(int a_Key, int a_ScanCode, int a_Modifiers)
	{
		switch (a_Key)
		{

		case GLFW_KEY_BACKSPACE:
			{
				m_TextField->PopCharacter();

			} break;

		case GLFW_KEY_ENTER:
			{
				m_TextField->AddCharacter((unsigned int)'\n');

			} break;

		}
	}

private:

	exl::Library* m_Library;
	exl::FontLoaderFreetype* m_FontLoader;
	exl::IFont* m_Font;
	exl::FontFace* m_FontFace;

	TextField* m_TextField;

}; // class ExampleTextField

int main(int argc, const char** argv)
{
	ExampleTextField application(argc, argv);
	return application.Run();
}