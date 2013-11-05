#pragma once

#include "IRenderCommand.h"

#include <Line.h>

namespace Framework
{
	class ShaderProgram;
}

namespace Framework
{

	class RenderCommandLines
		: public IRenderCommand
	{

	public:

		struct RenderState
		{
			ShaderProgram* program;

			GLint attribute_position;
			GLint uniform_modelviewprojection;
			GLint uniform_color;
		};

		static RenderState* CreateRenderState();
	
	public:
	
		RenderCommandLines(RenderState* a_State);
		~RenderCommandLines();

		void SetColor(const glm::vec4& a_Color);

		void SetThickness(float a_Thickness);
	
		void AddLine(const glm::vec2& a_Start, const glm::vec2& a_End);

		void Batch();

		void Render(const glm::mat4x4& a_Projection) const;

	private:

		RenderState* m_State;

		std::vector<ExLibris::Line> m_Lines;
		glm::vec4 m_Color;
		float m_Thickness;

		GLuint m_VertexBuffer;
		GLuint m_ElementBuffer;
		GLuint m_VertexArrayBuffer;
	
	}; // class RenderCommandLines

}; // namespace Framework