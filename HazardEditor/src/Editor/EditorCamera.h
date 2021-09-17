#pragma once

#include "Hazard.h"

using namespace Hazard;

namespace Editor {
	class EditorCamera {
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspecRatio, float nearPlane, float farPlane);
		~EditorCamera() = default;

		void OnUpdate();
		bool OnEvent(Event& e);

		inline float GetDistance() const { return distance; }
		inline void SetDistance(float distance) { this->distance = distance; }

		inline void SetViewpotSize(float w, float h) { viewport_w = w; viewport_h = h; UpdateProjection(); }

		const glm::mat4& GetView() const { return viewMatrix; }
		const glm::mat4& GetProjection() const { return projectionMatrix; }
		glm::mat4 GetViewPprojection() const { return projectionMatrix * viewMatrix; }

		glm::quat GetOrientation() const;
		const void SetMousePosition(glm::vec2 pos) { m_InitialMousePos = pos; }

		void SetIs2D(bool enabled2D);
		float GetPitch() const { return pitch; }
		float GetYaw() const { return yaw; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		const bool Is2DEnabled() const { return m_Is2DEnabled; }

	private:
		float fov = 45.0f, aspectRatio = 1.778f, nearPlane = 0.1f, farPlane = 1000.0f;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		bool m_Is2DEnabled;
		float size2D = 10.0f;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_InitialMousePos = { 0, 0 };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		float pitch = 0, yaw = 0;
		float distance = 10.0f;

		float viewport_w = 1280, viewport_h = 720;

		void UpdateProjection();
	private:
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