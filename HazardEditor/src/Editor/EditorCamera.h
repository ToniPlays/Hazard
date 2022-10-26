#pragma once

#include <hzrpch.h>
#include "Renderer/Camera.h"

using namespace Hazard;

namespace Editor 
{
	enum class CameraMode
	{
		None, FlyCam, Arcball
	};

	class EditorCamera : public HazardRenderer::Camera {
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspecRatio, float nearPlane, float farPlane);
		~EditorCamera() = default;

		void Init();

		glm::mat4 GetProjection() const override { return m_Projection; };
		glm::mat4 GetView() const override { return m_ViewMatrix; };
		glm::vec3 GetPosition() const override { return m_Position; };

		const float GetNearClipping() const override { return m_ZNear; };
		const float GetFarClipping() const override { return m_ZFar; };

		void OnUpdate();
		bool OnEvent(Event& e);

		float GetCameraSpeed() const;
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
		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		const bool Is2DEnabled() const { return m_Is2DEnabled; }

	private:
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = glm::vec3(0.0f), m_Direction = glm::vec3(0.0f), m_FocalPoint = glm::vec3(0.0f);

		float viewport_w = 1280, viewport_h = 720;

		float fov, aspectRatio, m_ZNear, m_ZFar;

		glm::vec2 m_InitialMousePos = {};
		glm::vec3 m_InitialFocalPoint = {}, m_InitialRotation = {};

		float m_Distance = 1.0f;
		float m_NormalSpeed = 2.5f;
		float m_Pitch = 0, m_Yaw = 0;
		float m_PitchDelta = 0.0f, m_YawDelta = 0.0f;

		glm::vec3 m_PositionDelta = glm::vec3(0.0f), m_RightDirection = glm::vec3(0.0f);

		float m_MinFocusDistance = 100.0f;
		CameraMode m_CameraMode = CameraMode::Arcball;

		constexpr static float MIN_SPEED = 0.5f, MAX_SPEED = 50.0f;

		float size2D = 10.0f;
		bool m_Is2DEnabled = false;

		glm::mat4 m_Projection = glm::mat4(1.0f);

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