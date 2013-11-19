// STL

#include <sstream>
#include <string>
#include <map>
#include <vector>

// GLM

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

// Framework

#include <Application.h>
#include <DebugHelper.h>
#include <MeshOpenGL.h>
#include <ShaderProgram.h>

namespace fw = Framework;

// ExLibris

#include <FontLoaderFreetype.h>
#include <GlyphBitmap.h>
#include <ITextLayoutVisitor.h>
#include <Library.h>
#include <TextLayout.h>

namespace exl = ExLibris;

class GUILabel
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

	GUILabel(exl::Library* a_Library)
		: m_Library(a_Library)
		, m_Face(nullptr)
		, m_Layout(nullptr)
		, m_UseShadow(false)
		, m_UseGlow(false)
		, m_Texture(0)
		, m_TextureWidth(0)
		, m_TexturePitch(0)
		, m_TextureHeight(0)
		, m_TextureDimensions(0, 0)
		, m_TextureData(nullptr)
		, m_TexturePadding(8, 8)
		, m_ShaderProgram(nullptr)
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

		m_ShaderProgram = new fw::ShaderProgram();
		m_ShaderProgram->LoadSourceFromFile(GL_VERTEX_SHADER, "Shaders/TextEffects.vert");
		m_ShaderProgram->LoadSourceFromFile(GL_FRAGMENT_SHADER, "Shaders/TextEffects.frag");
		m_ShaderProgram->Link();

		glGenVertexArrays(1, &m_BufferAttributes);
		glBindVertexArray(m_BufferAttributes);

			glBindBuffer(GL_ARRAY_BUFFER, m_BufferVertices);

			GLint attribute_position = m_ShaderProgram->FindAttribute("attrPosition");
			glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (const GLvoid*)GlyphVertex::eOffset_Position);
			glEnableVertexAttribArray(attribute_position);

			GLint attribute_texturecoordinate = m_ShaderProgram->FindAttribute("attrTextureCoordinate0");
			glVertexAttribPointer(attribute_texturecoordinate, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (const GLvoid*)GlyphVertex::eOffset_TextureCoordinate);
			glEnableVertexAttribArray(attribute_texturecoordinate);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferElements);

		glBindVertexArray(0);
	}

	~GUILabel()
	{
		if (m_Face != nullptr)
		{
			delete m_Face;
		}

		delete m_Layout;

		glDeleteTextures(1, &m_Texture);
		glDeleteBuffers(1, &m_BufferVertices);
		glDeleteBuffers(1, &m_BufferElements);
		glDeleteVertexArrays(1, &m_BufferAttributes);
		delete m_ShaderProgram;
	}

	void SetPosition(const glm::vec2& a_Position)
	{
		m_Position = a_Position;
	}

	void SetFace(const exl::FaceRequest& a_Request)
	{
		if (m_Face != nullptr)
		{
			delete m_Face;
		}

		m_Face = m_Library->RequestFace(a_Request);
		m_Layout->SetFace(m_Face);

		m_Layout->Accept(*this);
	}

	void SetText(const std::string& a_Text)
	{
		m_Layout->SetText(a_Text);

		m_Layout->Accept(*this);
	}

	void SetUseShadow(bool a_Use)
	{
		m_UseShadow = a_Use;
	}

	void SetUseGlow(bool a_Use)
	{
		m_UseGlow = a_Use;
	}

	void Render(int a_ViewportWidth, int a_ViewportHeight)
	{
		glm::vec2 screen_position = m_Position + m_RenderCorrection;

		glm::mat4x4 modelview;
		modelview = glm::translate(modelview, glm::vec3(screen_position, 0.0f));
		modelview = glm::scale(modelview, glm::vec3(m_TextureDimensions, 1.0f));

		glm::mat4x4 projection = glm::ortho<float>(
			0.0f, (float)a_ViewportWidth,
			(float)a_ViewportHeight, 0.0f,
			-1.0f, 1.0f
		);

		glm::mat4x4 mvp = projection * modelview;

		glUseProgram(m_ShaderProgram->GetHandle());
		glUniform1i(m_ShaderProgram->FindUniform("uniUseShadow"), m_UseShadow ? GL_TRUE : GL_FALSE);
		glUniform1i(m_ShaderProgram->FindUniform("uniUseGlow"), m_UseGlow ? GL_TRUE : GL_FALSE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// render text

		glUniformMatrix4fv(m_ShaderProgram->FindUniform("matModelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));

		glUniform2fv(m_ShaderProgram->FindUniform("uniTextureDimensions"), 1, glm::value_ptr(m_TextureDimensions));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glUniform1i(m_ShaderProgram->FindUniform("texTexture0"), 0);

		glBindVertexArray(m_BufferAttributes);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);

		glUseProgram(0);
	}

private:

	void VisitTextBegin(const exl::Face* a_Face, const exl::BoundingBox& a_BoundingBox)
	{
		m_TextureCorrection = -a_BoundingBox.GetMinimum() + glm::vec2(m_TexturePadding);
		m_RenderCorrection = -m_TextureCorrection;

		glm::vec2 dimensions = a_BoundingBox.GetDimensions();

		// texture must be padded in order to support effects like glow and shadows

		m_TextureWidth = (unsigned int)dimensions.x + (m_TexturePadding.x * 2);
		m_TexturePitch = m_TextureWidth * 4;
		m_TextureHeight = (unsigned int)dimensions.y + (m_TexturePadding.y * 2);

		m_TextureDimensions.x = (float)m_TextureWidth;
		m_TextureDimensions.y = (float)m_TextureHeight;

		m_TextureData = new unsigned char[m_TexturePitch * m_TextureHeight];

		// clear texture

		unsigned int clear_color = 0x00000000;

		for (unsigned int y = 0; y < m_TextureHeight; ++y)
		{
			unsigned int* dst = (unsigned int*)(m_TextureData + y * m_TexturePitch);

			for (unsigned int x = 0; x < m_TextureWidth; ++x)
			{
				*dst++ = clear_color;
			}
		}
	}

	void VisitTextLineBegin(const exl::TextLine* a_Line)
	{
	}

	void VisitTextCharacter(const exl::TextCharacter* a_Character)
	{
		glm::vec2 texture_position = a_Character->bounding_box.GetMinimum() + m_TextureCorrection;

		exl::GlyphBitmap* bitmap = m_Face->CreateBitmap(a_Character->identifier);
		if (bitmap != nullptr)
		{
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
	}

	void VisitTextWhitespace(const exl::TextCharacter* a_Character)
	{
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

	exl::Library* m_Library;

	exl::Face* m_Face;
	exl::TextLayout* m_Layout;

	glm::vec2 m_Position;
	glm::vec2 m_RenderCorrection;
	glm::vec2 m_TextureCorrection;

	GLuint m_Texture;
	unsigned int m_TextureWidth;
	unsigned int m_TexturePitch;
	unsigned int m_TextureHeight;
	glm::vec2 m_TextureDimensions;
	unsigned char* m_TextureData;
	glm::ivec2 m_TexturePadding;

	fw::ShaderProgram* m_ShaderProgram;
	GLuint m_BufferVertices;
	GLuint m_BufferElements;
	GLuint m_BufferAttributes;

	bool m_UseShadow;
	bool m_UseGlow;

}; // class GUILabel

class ExampleFaceSelection
	: public fw::Application
{

public:

	ExampleFaceSelection(int a_ArgumentCount, const char** a_Arguments)
		: fw::Application(a_ArgumentCount, a_Arguments)
		, m_Library(nullptr)
	{
	}

	~ExampleFaceSelection()
	{
	}
	
	bool ParseCommandLine(int a_ArgumentCount, const char** a_Arguments)
	{
		return true;
	}

	bool Initialize()
	{
		m_Library = new exl::Library;
		m_Library->AddLoader(new exl::FontLoaderFreetype(m_Library));
		m_Library->MapFontToFile("Fonts/Roboto/Roboto-Regular.ttf");
		m_Library->MapFontToFile("Fonts/Mathilde/mathilde.otf");
		m_Library->MapFontToFile("Fonts/00_starmap/00.fon");
		m_Library->MapFontToFile("Fonts/Entypo/Entypo.otf");

		CreateLabel("System (default bitmap font)", "System", 24.0f, glm::vec2(20.0f, 20.0f));
		CreateLabel("Roboto (TrueType vector font)", "Roboto", 24.0f, glm::vec2(20.0f, 80.0f));
		CreateLabel("00 Starmap (TrueType bitmap font)", "00 Starmap", 8.0f, glm::vec2(20.0f, 120.0f));
		CreateLabel("Mathilde (OpenType vector font)", "Mathilde", 60.0f, glm::vec2(20.0f, 180.0f));

		return true;
	}

	void CreateLabel(const std::string& a_Text, const std::string& a_Family, float a_Size, const glm::vec2& a_Position)
	{
		GUILabel* label = new GUILabel(m_Library);

		exl::FaceRequest label_request;
		label_request.SetFamilyName(a_Family);
		label_request.SetSize(a_Size);
		label->SetFace(label_request);

		label->SetText(a_Text);
		label->SetPosition(a_Position);

		m_Labels.push_back(label);
	}

	void Update(float a_DeltaTime)
	{
	}

	void Render()
	{
		int width, height;
		glfwGetFramebufferSize(GetWindow(), &width, &height);

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (std::vector<GUILabel*>::iterator label_it = m_Labels.begin(); label_it != m_Labels.end(); ++label_it)
		{
			GUILabel* label = *label_it;

			label->Render(width, height);
		}
	}

	void Destroy()
	{
		delete m_Library;

		for (std::vector<GUILabel*>::iterator label_it = m_Labels.begin(); label_it != m_Labels.end(); ++label_it)
		{
			delete *label_it;
		}
	}

private:

	exl::Library* m_Library;
	std::vector<GUILabel*> m_Labels;

}; // class ExampleFaceSelection


int main(int argc, const char** argv)
{
	ExampleFaceSelection application(argc, argv);
	return application.Run();
}