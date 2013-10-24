#ifndef _TEXTHELPER_H_
#define _TEXTHELPER_H_

#include <glm/glm.hpp>

namespace ExLibris
{
	class FontFace;
	class IFont;
}

namespace Framework
{
	class TextLabel;
}

namespace Framework
{

	class TextHelper
	{
	
	public:
	
		TextHelper();
		~TextHelper();
	
		void Clear();
		void AddText(const std::string& a_Text, const glm::vec2& a_Position, const glm::vec4& a_Color = glm::vec4(1.0, 1.0, 1.0, 1.0));

		void Render(int a_ScreenWidth, int a_ScreenHeight) const;

	private:

		void _LoadShader();
		void _CreateVertexBuffer();
		void _CreateVertexArray();

	private:

		ExLibris::IFont* m_Font;
		ExLibris::FontFace* m_FontFace;

		std::vector<TextLabel*> m_Labels;

		GLuint m_ShaderVertex;
		GLuint m_ShaderFragment;
		GLuint m_Program;
		GLint m_AttributePosition;
		GLint m_AttributeTextureCoordinate0;
		GLint m_UniformModelViewProjection;
		GLint m_UniformTexture0;
		GLint m_UniformTextureDimensions;
		GLint m_UniformTextColor;

		GLuint m_BufferVertices;
		GLuint m_BufferElements;
		GLuint m_BufferAttributes;
	
	}; // class TextHelper

}; // namespace Framework

#endif