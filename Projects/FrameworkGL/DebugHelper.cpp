#include "FrameworkGL.PCH.h"

#include "DebugHelper.h"

// GLM

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ExLibris

#include <FontFace.h>
#include <Library.h>
#include <IFont.h>
#include <TextLayout.h>

// Framework

#include "ShaderProgram.h"

namespace Framework
{

	DebugHelper::DebugHelper(ExLibris::Library* a_Library)
		: m_Library(a_Library)
		, m_Font(nullptr)
		, m_RenderStateText(nullptr)
		, m_RenderStateLines(nullptr)
		, m_Color(1.0f, 1.0f, 1.0f, 1.0f)
		, m_Thickness(1.0f)
	{
		ExLibris::FaceRequest request;
		request.SetFamilyName("System");

		m_Font = m_Library->RequestFace(request);

		m_RenderStateText = RenderCommandText::CreateRenderState();
		m_RenderStateLines = RenderCommandLines::CreateRenderState();
	}
	
	DebugHelper::~DebugHelper()
	{
		Clear();

		delete m_RenderStateLines->program;
		delete m_RenderStateLines;

		delete m_RenderStateText->program;
		glDeleteBuffers(1, &m_RenderStateText->vertex_buffer);
		glDeleteBuffers(1, &m_RenderStateText->element_buffer);
		glDeleteVertexArrays(1, &m_RenderStateText->vertex_attribute_buffer);
		delete m_RenderStateText;
	}

	void DebugHelper::SetColor(const glm::vec4& a_Color)
	{
		m_Color = a_Color;
	}

	void DebugHelper::SetThickness(float a_Thickness)
	{
		m_Thickness = a_Thickness;
	}

	void DebugHelper::AddLine(const glm::vec2& a_Start, const glm::vec2& a_End)
	{
		RenderCommandLines* command = new RenderCommandLines(m_RenderStateLines);
		command->AddLine(a_Start, a_End);
		command->SetColor(m_Color);
		command->SetThickness(m_Thickness);
		command->Batch();

		m_Commands.push_back(command);
	}

	void DebugHelper::AddBox(const ExLibris::BoundingBox& a_Box)
	{
		glm::vec2 box_ul = a_Box.GetTopLeft();
		glm::vec2 box_ur = a_Box.GetTopRight();
		glm::vec2 box_ll = a_Box.GetBottomLeft();
		glm::vec2 box_lr = a_Box.GetBottomRight();

		RenderCommandLines* command = new RenderCommandLines(m_RenderStateLines);
		command->AddLine(box_ul, box_ur);
		command->AddLine(box_ur, box_lr);
		command->AddLine(box_lr, box_ll);
		command->AddLine(box_ll, box_ul);
		command->SetColor(m_Color);
		command->SetThickness(m_Thickness);
		command->Batch();

		m_Commands.push_back(command);
	}

	void DebugHelper::AddCircle(const glm::vec2& a_Position, float a_Radius)
	{
		RenderCommandLines* command = new RenderCommandLines(m_RenderStateLines);
		command->SetColor(m_Color);
		command->SetThickness(m_Thickness);

		int steps = 20;
		float degrees_step = 360.0f / (float)steps;
		float degrees = 0.0f;

		for (int i = 0; i < steps; ++i)
		{
			glm::vec2 position_current(
				a_Position.x + glm::cos(glm::radians(degrees)) * a_Radius,
				a_Position.y + glm::sin(glm::radians(degrees)) * a_Radius
			);

			glm::vec2 position_next(
				a_Position.x + glm::cos(glm::radians(degrees + degrees_step)) * a_Radius,
				a_Position.y + glm::sin(glm::radians(degrees + degrees_step)) * a_Radius
			);

			command->AddLine(position_current, position_next);

			degrees += degrees_step;
		}

		command->Batch();

		m_Commands.push_back(command);
	}
	
	void DebugHelper::AddText(const std::string& a_Text, const glm::vec2& a_Position)
	{
		RenderCommandText* command = new RenderCommandText(m_RenderStateText, m_Font);
		command->SetText(a_Text);
		command->SetPosition(a_Position);
		command->SetColor(m_Color);
		command->Batch();

		m_Commands.push_back(command);
	}

	void DebugHelper::Clear()
	{
		for (std::vector<IRenderCommand*>::const_iterator command_it = m_Commands.begin(); command_it != m_Commands.end(); ++command_it)
		{
			IRenderCommand* command = *command_it;

			delete command;
		}
		m_Commands.clear();
	}

	void DebugHelper::Render(int a_ScreenWidth, int a_ScreenHeight) const
	{
		if (m_Commands.size() == 0)
		{
			return;
		}

		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, a_ScreenWidth, a_ScreenHeight);

		glm::mat4x4 projection = glm::ortho<float>(
			0.0f, (float)a_ScreenWidth,
			(float)a_ScreenHeight, 0.0f,
			-1.0f, 1.0f
		);

		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (std::vector<IRenderCommand*>::const_iterator command_it = m_Commands.begin(); command_it != m_Commands.end(); ++command_it)
		{
			IRenderCommand* command = *command_it;

			command->Render(projection);
		}

		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);

		glPopAttrib();
	}

}; // namespace Framework