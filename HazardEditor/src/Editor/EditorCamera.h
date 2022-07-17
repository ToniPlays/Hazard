#pragma once

#include <hzrpch.h>
#include "Renderer/Camera.h"

using namespace Hazard;

namespace Editor {
	class EditorCamera : public HazardRenderer::Camera {
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspecRatio, float nearPlane, float farPlane);
		~EditorCamera() = default;

		glm::mat4 GetProjection() const override { return m_Projection; };
		glm::mat4 GetView() const override { return m_View; };
		glm::mat4 GetViewProjection() const override { return m_Projection * m_View; };
		glm::vec3 GetPosition() const override { return m_Position; };

		const float GetNearClipping() const override { return m_ZNear; };
		const float GetFarClipping() const override { return m_ZFar; };


		void OnUpdate();
		bool OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewport(float width, float height) { viewport_w = width; viewport_h = height; UpdateProjection(); }
		
		glm::quat GetOrientation() const;

		const void SetMousePosition(const glm::vec2& pos) { m_InitialMousePos = pos; }

		void SetIs2D(bool enabled2D);
		void SetFocalPoint(const glm::vec3& point) {
			m_FocalPoint = point;
			UpdateView();
		};
		float GetPitch() const { return pitch; }
		float GetYaw() const { return yaw; }

		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const bool Is2DEnabled() const { return m_Is2DEnabled; }

	private:
		float viewport_w = 1280, viewport_h = 720;
		float fov = 45.0f, aspectRatio = 1.778f;
		float pitch = 0, yaw = 0;
		float m_ZNear, m_ZFar;

		float m_Distance = 10.0f;
		float size2D = 10.0f;
		bool m_Is2DEnabled = false;

		glm::vec2 m_InitialMousePos = { 0, 0 };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec3 m_Position = { 0, 0, 0 };

		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

		glm::vec3 CalculatePosition() const;
	};
}