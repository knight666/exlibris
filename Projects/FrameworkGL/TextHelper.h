#pragma once

#include <glm/glm.hpp>

#include "BoundingBox.h"
#include "RenderCommandLines.h"
#include "RenderCommandText.h"

namespace ExLibris
{
	class FontFace;
	class IFont;
}

namespace Framework
{

	class TextHelper
	{
	
	public:
	
		TextHelper();
		~TextHelper();
	
		void Clear();

		void AddText(const std::string& a_Text, const glm::vec2& a_Position, const glm::vec4& a_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		void AddLine(const glm::vec2& a_Start, const glm::vec2& a_End, float a_Thickness = 1.0f, const glm::vec4& a_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		void AddBox(const ExLibris::BoundingBox& a_Box, float a_Thickness = 1.0f, const glm::vec4& a_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		void Render(int a_ScreenWidth, int a_ScreenHeight) const;

	private:

		void _LoadShader(
			GLuint& a_Program,
			GLuint& a_VertexShader, const std::string& a_VertexSource,
			GLuint& a_FragmentShader, const std::string& a_FragmentSource
		);
		void _CreateTextState();
		void _CreateLinesState();

	private:

		ExLibris::IFont* m_Font;
		ExLibris::FontFace* m_FontFace;

		RenderCommandText::RenderState* m_RenderStateText;
		RenderCommandLines::RenderState* m_RenderStateLines;

		std::vector<IRenderCommand*> m_Commands;

	}; // class TextHelper

}; // namespace Framework