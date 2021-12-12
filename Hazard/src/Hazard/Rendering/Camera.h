#pragma once

#include "Hazard/Core/Core.h"
#include <glm/glm.hpp>

namespace Hazard::Rendering {
	class Camera 
	{
	public:
		Camera() = default;

		virtual const glm::mat4& GetProjection() const { return m_Projection; };
		virtual const glm::mat4& GetView() const { return m_View; };
		virtual glm::mat4 GetViewPprojection() const { return m_Projection * m_View; };
		virtual void SetView(const glm::mat4& view) { m_View = view; }
		virtual void SetProjection(const glm::mat4& projection) { m_Projection = projection; }
		virtual void SetViewport(uint32_t width, uint32_t heigth) {};

	protected:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
	};
}