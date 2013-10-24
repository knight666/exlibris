#ifndef _TEXTHELPER_H_
#define _TEXTHELPER_H_

#include <glm/glm.hpp>

namespace ExLibris
{
	class FontFace;
	class IFont;
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

		void Render(int a_ScreenWidth, int a_ScreenHeight) const;

	private:

		ExLibris::IFont* m_Font;
		ExLibris::FontFace* m_FontFace;

		std::vector<TextLabel*> m_Labels;

		ShaderProgram* m_Program;
		GLuint m_BufferVertices;
		GLuint m_BufferElements;
		GLuint m_BufferAttributes;
	
	}; // class TextHelper

}; // namespace Framework

#endif