#pragma once

#include <ITextLayoutVisitor.h>

#include "IRenderCommand.h"

namespace ExLibris
{
	class FontFace;
	class TextLayout;
}

namespace Framework
{

	class RenderCommandText
		: public IRenderCommand
		, public ExLibris::ITextLayoutVisitor
	{

	public:

		struct RenderState
		{
			GLuint vertex_shader;
			GLuint fragment_shader;
			GLuint program;

			GLint uniform_modelviewprojection;
			GLint uniform_texture0;
			GLint uniform_texturedimensions;
			GLint uniform_textcolor;

			GLuint vertex_buffer;
			GLuint element_buffer;
			GLuint vertex_attribute_buffer;
		};
	
	public:
	
		RenderCommandText(RenderState* a_State, ExLibris::FontFace* a_Face);
		~RenderCommandText();

		void SetText(const std::string& a_Text);
	
		void SetPosition(const glm::vec2& a_Position);

		void SetColor(const glm::vec4& a_Color);
		
		void Batch();

		void Render(const glm::mat4x4& a_Projection) const;

	public:

		void VisitTextBegin(const ExLibris::FontFace* a_Face, const glm::vec2& a_Dimensions);
		void VisitTextLineBegin(size_t a_GlyphCount, const glm::vec2& a_Offset, float a_Width);
		void VisitTextCharacter(const ExLibris::Glyph* a_Glyph, float a_X, float a_Advance);
		void VisitTextWhitespace(unsigned int a_Identifier, float a_X, float a_Advance);
		void VisitTextLineEnd();
		void VisitTextEnd();

	private:

		RenderState* m_State;

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
	
	}; // class RenderCommandText

}; // namespace Framework