#pragma once

#include <glm/glm.hpp>

namespace Framework
{

	class IRenderCommand
	{
	
	public:

		virtual ~IRenderCommand()
		{
		}

		virtual void Batch() = 0;
	
		virtual void Render(const glm::mat4x4& a_Projection) const = 0;
	
	}; // class IRenderCommand

}; // namespace Framework