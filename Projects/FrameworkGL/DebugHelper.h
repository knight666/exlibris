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

	class DebugHelper
	{
	
	public:
	
		DebugHelper();
		~DebugHelper();
	
		void Clear();

		void SetColor(const glm::vec4& a_Color);

		void SetThickness(float a_Thickness);

		void AddLine(const glm::vec2& a_Start, const glm::vec2& a_End);
		void AddBox(const ExLibris::BoundingBox& a_Box);
		void AddCircle(const glm::vec2& a_Position, float a_Radius);

		void AddText(const std::string& a_Text, const glm::vec2& a_Position);

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

		glm::vec4 m_Color;
		float m_Thickness;

	}; // class TextHelper

}; // namespace Framework