#pragma once

#include "Hazard/Math/Color.h"
#include "Hazard/Rendering/Sky/BackgroundRenderer.h"

namespace Hazard::ECS 
{
	enum Projection { Perspective = 0, Orthographic };
	enum Background { SingleColor = 0, Skybox };

	struct CameraComponent {

	public:
		Rendering::BackgroundRenderer* bgRenderer = new Rendering::SkyboxBackground(
			Rendering::RenderUtils::Create<Rendering::CubemapTexture>("res/textures/sea-", ".jpg"));
			
		Projection GetProjectionType() { return m_Type; }
		Background GetBackgroundType() { return m_BackgroundType; }

		float GetFov() { return m_Fov; }
		glm::mat4 GetProjection() { return m_Projection; }
		glm::vec2 GetClipping() { return { m_ZNear, m_ZFar }; }

		void SetProjection(Projection t);
		void SetBackground(Background t);
		void SetFov(float fov);
		void SetZNear(float plane);
		void SetZFar(float plane);
		void RecalculateProjection(float w, float h);

	private:

		Projection m_Type = Projection::Perspective;
		Background m_BackgroundType = Background::SingleColor;
		glm::mat4 m_Projection;

		float m_Fov = 10.0f;
		float m_Width = 1920, m_Height = 1080, m_ZNear = 0.03f, m_ZFar = 1000.0f;
	};
}