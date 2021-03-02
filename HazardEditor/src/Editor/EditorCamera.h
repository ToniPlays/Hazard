#pragma once

#include "glm/glm.hpp"
#include "Hazard.h"

using namespace Hazard;

namespace Editor {
	class EditorCamera {
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspecRatio, float nearPlane, float farPlane);
		~EditorCamera();

		void OnUpdate();
		bool OnEvent(Event& e);

		inline float GetDistance() const { return distance; }
		inline float SetDistance(float distance) { this->distance = distance; }

		inline void SetViewpotSize(float w, float h) { viewport_w = w; viewport_h = h; }

		const glm::mat4& GetView() const { return viewMatrix; }
		const glm::mat4& GetProjection() const { return projectionMatrix; }
		glm::mat4 GetViewPprojection() const { return projectionMatrix * viewMatrix; }

		glm::quat GetOrientation() const;
		const void SetMousePosition(glm::vec2 pos) { initialMousePos = pos; }

		float GetPitch() const { return pitch; }
		float GetYaw() const { return yaw; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return position; }

	private:
		float fov = 45.0f, aspectRatio = 1.778f, nearPlane = 0.1f, farPlane = 1000.0f;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 initialMousePos = { 0, 0 };
		glm::vec3 focalPoint = { 0.0f, 0.0f, 0.0f };

		float pitch = 0, yaw = 0;
		float distance = 10.0f;

		float viewport_w = 1280, viewport_h = 720;

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