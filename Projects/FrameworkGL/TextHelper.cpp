#include "FrameworkGL.PCH.h"

#include "TextHelper.h"

// Windows

#include <ShlObj.h>

// GLM

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ExLibris

#include <FontFace.h>
#include <IFont.h>
#include <Library.h>
#include <TextLayout.h>

// Framework

#include "ShaderLoader.h"
#include "ShaderProgram.h"

namespace Framework
{

	class TextLabel
		: public ExLibris::ITextLayoutVisitor
	{

	public:

		TextLabel(ExLibris::FontFace* a_Font)
			: m_Font(a_Font)
			, m_Layout(nullptr)
			, m_Texture(0)
			, m_TextureWidth(0)
			, m_TexturePitch(0)
			, m_TextureHeight(0)
			, m_TextureData(nullptr)
			, m_TexturePadding(2, 2)
		{
			m_RenderCorrection.y = -m_Font->GetAscender();

			glGenTextures(1, &m_Texture);
			glBindTexture(GL_TEXTURE_2D, m_Texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);

			m_Layout = new ExLibris::TextLayout;
			m_Layout->SetFontFace(m_Font);
		}

		~TextLabel()
		{
			delete m_Layout;

			glDeleteTextures(1, &m_Texture);
		}

		void SetText(const std::string& a_Text)
		{
			m_Layout->SetText(a_Text);
			m_Layout->Accept(*this);
		}

		void SetPosition(const glm::vec2& a_Position)
		{
			m_Position = a_Position;
		}

		const glm::mat4x4& GetModelviewMatrix() const
		{
			return m_MatrixModelview;
		}

		GLuint GetTexture() const
		{
			return m_Texture;
		}

		const glm::vec2& GetTextureDimensions() const
		{
			return m_TextureDimensions;
		}

		const glm::vec4& GetColor() const
		{
			return m_Color;
		}

		void SetColor(const glm::vec4& a_Color)
		{
			m_Color = a_Color;
		}

	private:

		void VisitTextBegin(const ExLibris::FontFace* a_Face, const glm::vec2& a_Dimensions)
		{
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

		void VisitTextLineBegin(size_t a_GlyphCount, const glm::vec2& a_Offset, float a_Width)
		{
			m_LineOffset = a_Offset;
			m_LineOffset.y += m_Font->GetDescender();

			m_CursorPosition = a_Offset;

			m_LineCorrection.x = 0.0f;
			m_LineCorrection.y = 0.0f;
		}

		void VisitTextCharacter(const ExLibris::Glyph* a_Glyph, float a_X, float a_Advance)
		{
			ExLibris::GlyphMetrics* metrics = a_Glyph->metrics;
			ExLibris::GlyphBitmap* bitmap = a_Glyph->bitmap;

			m_CursorPosition.x = a_X + a_Advance;

			glm::vec2 offset = m_LineOffset + metrics->offset;
			offset.x += a_X;

			glm::vec2 shadow_offset = offset + glm::vec2(2.0f, 1.0f);

			m_RenderCorrection.x = std::min(m_RenderCorrection.x, offset.x);

			if (offset.x < 0.0f)
			{
				m_LineCorrection.x = fabs(offset.x);
				offset.x = 0.0f;
			}

			unsigned char* dst = m_TextureData + (((unsigned int)offset.y + m_TexturePadding.y) * m_TexturePitch) + ((unsigned int)(offset.x + m_TexturePadding.x) * 4);

			unsigned char* dst_shadow =
				m_TextureData + 
				(((unsigned int)shadow_offset.y + m_TexturePadding.y) * m_TexturePitch) + 
				((unsigned int)(shadow_offset.x + m_TexturePadding.x) * 4);

			unsigned char* dst_end = m_TextureData + m_TexturePitch * m_TextureHeight;

			unsigned int src_pitch = bitmap->width * 4;
			unsigned char* src = bitmap->data;

			for (unsigned int y = 0; y < bitmap->height; ++y)
			{
				/*if (dst_shadow >= m_TextureData && dst_shadow + src_pitch < dst_end)
				{
					unsigned int* dst_line = (unsigned int*)dst_shadow;
					unsigned int* src_line = (unsigned int*)src;

					for (unsigned int x = 0; x < bitmap->width; ++x)
					{
						unsigned int src_a = (*src_line & 0x000000FF);

						if (src_a > 0)
						{
							*dst_line = 0x313131FF;
						}

						++dst_line;
						++src_line;
					}

					dst_shadow += m_TexturePitch;
				}*/

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
				}

				src += src_pitch;
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

			glm::vec2 screen_position = m_Position + m_RenderCorrection + m_LineCorrection;

			m_MatrixModelview = glm::mat4x4();
			m_MatrixModelview = glm::translate(m_MatrixModelview, glm::vec3(screen_position.x, screen_position.y, 0.0f));
			m_MatrixModelview = glm::scale(m_MatrixModelview, glm::vec3((float)m_TextureWidth, (float)m_TextureHeight, 1.0f));
		}

	private:

		ExLibris::FontFace* m_Font;
		ExLibris::TextLayout* m_Layout;

		glm::vec2 m_Position;
		glm::mat4x4 m_MatrixModelview;

		glm::vec4 m_Color;

		glm::vec2 m_LineOffset;
		glm::vec2 m_LineCorrection;
		glm::vec2 m_RenderCorrection;

		glm::vec2 m_CursorPosition;

		GLuint m_Texture;
		unsigned int m_TextureWidth;
		unsigned int m_TexturePitch;
		unsigned int m_TextureHeight;
		glm::vec2 m_TextureDimensions;
		unsigned char* m_TextureData;
		glm::ivec2 m_TexturePadding;

	}; // class TextField

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

	TextHelper::TextHelper(ExLibris::Library* a_Library, ShaderLoader* a_ShaderLoader)
		: m_Program(nullptr)
		, m_Font(nullptr)
	{
		ExLibris::IFont* font = a_Library->LoadFont("Fonts/Roboto/Roboto-Regular.ttf");
		m_Font = font->CreateFace(10.0f);

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

		m_Program = a_ShaderLoader->LoadProgram("TextHelper", "Shaders/TextHelper");
		m_Program->Compile();
		m_Program->Link();

		glGenVertexArrays(1, &m_BufferAttributes);
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
	
	TextHelper::~TextHelper()
	{
		delete m_Program;

		for (std::vector<TextLabel*>::iterator label_it = m_Labels.begin(); label_it != m_Labels.end(); ++label_it)
		{
			TextLabel* label = *label_it;

			delete label;
		}
		m_Labels.clear();

		glDeleteBuffers(1, &m_BufferVertices);
		glDeleteBuffers(1, &m_BufferElements);
		glDeleteVertexArrays(1, &m_BufferAttributes);
	}

	void TextHelper::AddText(const std::string& a_Text, const glm::vec2& a_Position, const glm::vec4& a_Color /*= glm::vec4(1.0, 1.0, 1.0, 1.0)*/)
	{
		TextLabel* label = new TextLabel(m_Font);
		label->SetPosition(a_Position);
		label->SetText(a_Text);
		label->SetColor(a_Color);

		m_Labels.push_back(label);
	}

	void TextHelper::Clear()
	{
		for (std::vector<TextLabel*>::iterator label_it = m_Labels.begin(); label_it != m_Labels.end(); ++label_it)
		{
			TextLabel* label = *label_it;

			delete label;
		}
		m_Labels.clear();
	}

	void TextHelper::Render(const glm::mat4x4& a_ProjectionMatrix)
	{
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(m_Program->GetHandle());

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_Program->GetUniform("texTexture0"), 0);

		glBindVertexArray(m_BufferAttributes);

		for (std::vector<TextLabel*>::iterator label_it = m_Labels.begin(); label_it != m_Labels.end(); ++label_it)
		{
			TextLabel* label = *label_it;

			glBindTexture(GL_TEXTURE_2D, label->GetTexture());

			glUniform2fv(m_Program->GetUniform("uniTextureDimensions"), 1, glm::value_ptr(label->GetTextureDimensions()));

			glm::mat4x4 mvp = a_ProjectionMatrix * label->GetModelviewMatrix();
			glUniformMatrix4fv(m_Program->GetUniform("matModelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(0);
		
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);
	}

}; // namespace Framework