#ifndef _TEXTHELPER_H_
#define _TEXTHELPER_H_

#include <glm/glm.hpp>

namespace ExLibris
{
	class FontFace;
	class Library;
	class TextLayout;
}

namespace Framework
{
	class ShaderLoader;
	class ShaderProgram;
	class TextLabel;
}

namespace Framework
{

	class TextHelper
	{
	
	public:
	
		TextHelper(ExLibris::Library* a_Library, ShaderLoader* a_ShaderLoader);
		~TextHelper();
	
		void Clear();
		void AddText(const std::string& a_Text, const glm::vec2& a_Position, const glm::vec4& a_Color = glm::vec4(1.0, 1.0, 1.0, 1.0));

		void Render(const glm::mat4x4& a_ProjectionMatrix);

	private:

		ExLibris::FontFace* m_Font;

		std::vector<TextLabel*> m_Labels;

		ShaderProgram* m_Program;
		GLuint m_BufferVertices;
		GLuint m_BufferElements;
		GLuint m_BufferAttributes;
	
	}; // class TextHelper

}; // namespace Framework

#endif