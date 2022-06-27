#pragma once

#include <glm/glm.hpp>

namespace HazardRenderer {
	class Camera
	{
	public:
		Camera() = default;

		virtual const glm::mat4& GetProjection() const { return m_Projection; };
		virtual const glm::mat4& GetView() const { return m_View; };
		virtual glm::mat4 GetViewProjection() const { return m_Projection * m_View; };
		virtual void SetView(const glm::mat4& view) { m_View = view; }
		virtual void SetProjection(const glm::mat4& projection) { m_Projection = projection; }
		virtual void SetViewport(float width, float heigth) {};
		virtual const float GetNearClipping() const { return m_ZNear; }
		virtual const float GetFarClipping() const { return m_ZFar; }

	protected:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::vec3 m_Position;

		float m_ZNear = 0.03f;
		float m_ZFar = 100.0f;
	};
}
