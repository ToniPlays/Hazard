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
		void SetZNear(float plane);
		void SetZFar(float plane);
		float GetAspectRatio() { return m_Width / m_Height; }
		void RecalculateProjection(float w, float h);

		Projection GetProjectionType() { return m_Type; }
		float GetFov() { return m_Fov; }
		glm::mat4 GetProjection() { return m_Projection; }
		glm::vec2 GetClipping() { return { m_ZNear, m_ZFar }; }

	private:
		Projection m_Type = Projection::Perspective;
		glm::mat4 m_Projection;

		float m_Fov = 10.0f;
		float m_Width = 1920, m_Height = 1080, m_ZNear = 0.03f, m_ZFar = 1000.0f;
	};
}