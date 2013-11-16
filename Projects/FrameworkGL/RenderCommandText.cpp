#include "FrameworkGL.PCH.h"

#include "RenderCommandText.h"

// GLM

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ExLibris

#include <FontFace.h>
#include <IFont.h>
#include <TextLayout.h>

// Framework

#include "ShaderProgram.h"

namespace Framework
{
	
	static const std::string g_TextSourceVertex = "\
		#version 330 core\n \
		in vec2 attrPosition; \
		in vec2 attrTextureCoordinate0; \
		out vec2 vertTextureCoordinate; \
		uniform mat4 matModelViewProjection; \
		void main() \
		{ \
			gl_Position = matModelViewProjection * vec4(attrPosition, 0.0, 1.0); \
			vertTextureCoordinate = attrTextureCoordinate0; \
		}";

	static const std::string g_TextSourceFragment = "\
		#version 330 core\n \
		in vec2 vertTextureCoordinate; \
		uniform sampler2D texTexture0; \
		uniform vec2 uniTextureDimensions; \
		uniform vec4 uniTextColor; \
		out vec4 fragColor; \
		void main() \
		{ \
			vec4 color_sample = uniTextColor * texture(texTexture0, vertTextureCoordinate).a; \
			vec2 shadow_offset = vec2(-1.0, -1.0) / uniTextureDimensions; \
			vec4 shadow_sample = vec4(0.25, 0.25, 0.25, texture(texTexture0, vertTextureCoordinate + shadow_offset).a); \
			fragColor = mix(shadow_sample, color_sample, color_sample.a); \
		}";

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

	RenderCommandText::RenderState* RenderCommandText::CreateRenderState()
	{
		RenderState* state = new RenderState;

		state->program = ShaderProgram::Create(g_TextSourceVertex, g_TextSourceFragment);

		GLint attribute_position = state->program->FindAttribute("attrPosition");
		GLint attribute_texturecoordinate0 = state->program->FindAttribute("attrTextureCoordinate0");
		if (attribute_position == -1 || attribute_texturecoordinate0 == -1)
		{
			throw std::exception("Failed to find attribute locations.");
		}

		state->uniform_modelviewprojection = state->program->FindUniform("matModelViewProjection");
		state->uniform_texture0 = state->program->FindUniform("texTexture0");
		state->uniform_texturedimensions = state->program->FindUniform("uniTextureDimensions");
		state->uniform_textcolor = state->program->FindUniform("uniTextColor");
		if (state->uniform_modelviewprojection == -1 || state->uniform_texture0 == -1 || state->uniform_texturedimensions == -1 || state->uniform_textcolor == -1)
		{
			throw new std::exception("Failed to find uniform locations.");
		}

		// vertex buffer

		glGenBuffers(1, &state->vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, state->vertex_buffer);

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

		// element buffer

		glGenBuffers(1, &state->element_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state->element_buffer);

			GLuint element_data[6] = {
				1, 0, 2,
				1, 2, 3
			};

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), element_data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// vertex attribute buffer

		glGenVertexArrays(1, &state->vertex_attribute_buffer);
		glBindVertexArray(state->vertex_attribute_buffer);

			glBindBuffer(GL_ARRAY_BUFFER, state->vertex_buffer);

			glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (const GLvoid*)GlyphVertex::eOffset_Position);
			glEnableVertexAttribArray(attribute_position);

			glVertexAttribPointer(attribute_texturecoordinate0, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (const GLvoid*)GlyphVertex::eOffset_TextureCoordinate);
			glEnableVertexAttribArray(attribute_texturecoordinate0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state->element_buffer);

		glBindVertexArray(0);

		return state;
	}

	RenderCommandText::RenderCommandText(RenderState* a_State, ExLibris::FontFace* a_Face)
		: m_State(a_State)
		, m_Font(a_Face)
		, m_Layout(nullptr)
		, m_Texture(0)
		, m_TextureWidth(0)
		, m_TexturePitch(0)
		, m_TextureHeight(0)
		, m_TextureData(nullptr)
		, m_TexturePadding(2, 2)
	{
		m_RenderCorrection.y = -m_Font->GetAscender();

		m_Layout = new ExLibris::TextLayout;
		//m_Layout->SetFontFace(m_Font);
	}
	
	RenderCommandText::~RenderCommandText()
	{
		delete m_Layout;

		if (m_Texture != 0)
		{
			glDeleteTextures(1, &m_Texture);
		}
	}

	void RenderCommandText::SetText(const std::string& a_Text)
	{
		m_Layout->SetText(a_Text);
	}

	void RenderCommandText::SetPosition(const glm::vec2& a_Position)
	{
		m_Position = a_Position;
	}

	void RenderCommandText::SetColor(const glm::vec4& a_Color)
	{
		m_Color = a_Color;
	}

	void RenderCommandText::Batch()
	{
		m_Layout->Accept(*this);

		glm::vec2 screen_position = m_Position + m_RenderCorrection + m_LineCorrection;

		m_MatrixModelview = glm::mat4x4();
		m_MatrixModelview = glm::translate(m_MatrixModelview, glm::vec3(screen_position.x, screen_position.y, 0.0f));
		m_MatrixModelview = glm::scale(m_MatrixModelview, glm::vec3((float)m_TextureWidth, (float)m_TextureHeight, 1.0f));
	}

	void RenderCommandText::Render(const glm::mat4x4& a_Projection) const
	{
		glUseProgram(m_State->program->GetHandle());

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_State->uniform_texture0, 0);

		glBindVertexArray(m_State->vertex_attribute_buffer);

		glUniform4fv(m_State->uniform_textcolor, 1, glm::value_ptr(m_Color));

		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glUniform2fv(m_State->uniform_texturedimensions, 1, glm::value_ptr(m_TextureDimensions));

		glm::mat4x4 mvp = a_Projection * m_MatrixModelview;
		glUniformMatrix4fv(m_State->uniform_modelviewprojection, 1, GL_FALSE, glm::value_ptr(mvp));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void RenderCommandText::VisitTextBegin(const ExLibris::FontFace* a_Face, const glm::vec2& a_Dimensions, const ExLibris::BoundingBox& a_BoundingBox)
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

	void RenderCommandText::VisitTextLineBegin(size_t a_GlyphCount, const glm::vec2& a_Offset, float a_Width, const ExLibris::BoundingBox& a_BoundingBox)
	{
		m_LineOffset = a_Offset;
		m_LineOffset.y += m_Font->GetDescender();

		m_CursorPosition = a_Offset;

		m_LineCorrection.x = 0.0f;
		m_LineCorrection.y = 0.0f;
	}

	void RenderCommandText::VisitTextCharacter(const ExLibris::Glyph* a_Glyph, float a_X, float a_Advance, const ExLibris::BoundingBox& a_BoundingBox)
	{
		ExLibris::GlyphMetrics* metrics = a_Glyph->metrics;
		ExLibris::GlyphBitmap* bitmap = a_Glyph->bitmap;

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
			}

			src += src_pitch;
		}
	}

	void RenderCommandText::VisitTextWhitespace(unsigned int a_Identifier, float a_X, float a_Advance, const ExLibris::BoundingBox& a_BoundingBox)
	{
		m_CursorPosition.x = a_X + a_Advance;
	}

	void RenderCommandText::VisitTextLineEnd()
	{
	}

	void RenderCommandText::VisitTextEnd()
	{
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

}; // namespace Framework