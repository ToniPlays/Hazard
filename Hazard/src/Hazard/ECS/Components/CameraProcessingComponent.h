#pragma once

#include "Hazard/Core/Core.h"
#include "ComponentBase.h"

namespace Hazard
{
	enum class Projection { Perspective = 0, Orthographic };

	struct CameraComponent : ComponentBase {

	public:

		void SetProjection(Projection t);
		void SetFov(float fov);
		void SetSize(float size);
		void SetZNear(float plane);
		void SetZFar(float plane);
		void SetClipping(const glm::vec2 clipping);
		float GetAspectRatio() { return m_Width / m_Height; }
		void RecalculateProjection(float w, float h);

		Projection GetProjectionType() { return m_Type; }
		float GetFov() { return m_Fov; }
		float GetSize() { return m_Size; }
		glm::mat4 GetProjection();
		glm::vec2 GetClipping() { return { m_ZNear, m_ZFar }; }

	private:
		Projection m_Type = Projection::Perspective;
		glm::mat4 m_Projection = glm::mat4(1.0f);

		float m_Fov = 60.0f;
		float m_Size = 5.0f;
		float m_Width = 1920, m_Height = 1080, m_ZNear = 0.03f, m_ZFar = 1000.0f;
		bool m_Dirty = true;
	};
}