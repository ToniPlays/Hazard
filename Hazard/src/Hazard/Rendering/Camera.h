#pragma once

#include "Hazard/Core/Core.h"
#include <glm/glm.hpp>

namespace Hazard::Rendering {
	class Camera
	{
	public:
		Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; };
		const glm::mat4& GetView() const { return m_View; };
		glm::mat4 GetViewPprojection() const { return m_Projection * m_View; };
		const glm::vec3& GetPosition() const { return m_Position; }
		virtual void SetView(const glm::mat4& view) { m_View = view; }
		virtual void SetProjection(const glm::mat4& projection) { m_Projection = projection; }
		virtual void SetViewport(float width, float heigth) {};
		virtual const float GetNearClipping() const { return m_ZNear; }
		virtual const float GetFarClipping() const { return m_ZFar; }

	protected:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		float m_ZNear = 0.03f;
		float m_ZFar = 100.0f;
	};
}