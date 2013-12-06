#pragma once

#include "IRenderCommand.h"

namespace ExLibris
{
	class Face;
	struct TextCharacter;
	class TextLayout;
}

namespace Framework
{
	class ShaderProgram;
}

namespace Framework
{

	class RenderCommandText
		: public IRenderCommand
	{

	public:

		struct RenderState
		{
			ShaderProgram* program;

			GLint uniform_modelviewprojection;
			GLint uniform_texture0;
			GLint uniform_texturedimensions;
			GLint uniform_textcolor;

			GLuint vertex_buffer;
			GLuint element_buffer;
			GLuint vertex_attribute_buffer;
		};

		static RenderState* CreateRenderState();
	
	public:
	
		RenderCommandText(RenderState* a_State, ExLibris::Face* a_Face);
		~RenderCommandText();

		void SetText(const std::string& a_Text);
	
		void SetPosition(const glm::vec2& a_Position);

		void SetColor(const glm::vec4& a_Color);
		
		void Batch();

		void Render(const glm::mat4x4& a_Projection) const;

	private:

		void _BuildTexture();
		void _AddCharacterToTexture(ExLibris::TextCharacter* a_Character);

	private:

		RenderState* m_State;

		ExLibris::Face* m_Face;
		ExLibris::TextLayout* m_Layout;

		glm::vec2 m_Position;
		glm::mat4x4 m_MatrixModelview;

		glm::vec4 m_Color;

		glm::vec2 m_TextureCorrection;
		glm::vec2 m_RenderCorrection;

		GLuint m_Texture;
		unsigned int m_TextureWidth;
		unsigned int m_TexturePitch;
		unsigned int m_TextureHeight;
		glm::vec2 m_TextureDimensions;
		unsigned char* m_TextureData;
		glm::ivec2 m_TexturePadding;
	
	}; // class RenderCommandText

}; // namespace Framework