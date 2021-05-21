#pragma once

#include <hzreditor.h>
#include "EditorCamera.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Editor {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearPlane, float farPlane) : fov(fov), 
		aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane)
	{
		projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		UpdateView();
	}

	EditorCamera::~EditorCamera()
	{

	}

	void EditorCamera::OnUpdate()
	{
		const glm::vec2& mouse = Input::GetMousePos();
		glm::vec2 delta = (mouse - m_InitialMousePos) * 0.003f;
		m_InitialMousePos = mouse;

		if (Input::IsMouseButtonDown(Mouse::ButtonLeft) && !m_Is2DEnabled) {
			MouseRotate(delta);
		}
		else if (Input::IsMouseButtonDown(Mouse::ButtonRight))
			MousePan(delta);

		UpdateView();
	}

	bool EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT(EditorCamera::OnMouseScroll));
		return true;
	}

	void EditorCamera::UpdateProjection()
	{
		aspectRatio = viewport_w / viewport_h;
		SetIs2D(m_Is2DEnabled);
	}

	void EditorCamera::UpdateView()
	{
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		viewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		viewMatrix = glm::inverse(viewMatrix);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();

		if (m_Is2DEnabled) {
			xSpeed *= size2D * 0.09f;
			ySpeed *= size2D * 0.09f;
		}

		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		yaw += yawSign * delta.x * RotationSpeed();
		pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		if (m_Is2DEnabled) {
			size2D -= delta * ZoomSpeed();
			size2D = Math::Max(0.01f, size2D);
			SetIs2D(true);
			return;
		}
		
		distance -= delta * ZoomSpeed();
		if (distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			distance = 1.0f;
		}
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(viewport_w / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(viewport_h / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float dist = distance * 0.2f;
		dist = std::max(dist, 0.0f);
		float speed = dist * dist;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * distance;
	}
	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
	}
	void EditorCamera::SetIs2D(bool enabled2D)
	{

		if (enabled2D) {
			float side = aspectRatio * size2D;
			projectionMatrix = glm::ortho(-side, side, -size2D, size2D, nearPlane, farPlane);
			if (enabled2D != m_Is2DEnabled) 
			{
				pitch = 0;
				yaw = 0;
			}
		}
		else 
			projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		m_Is2DEnabled = enabled2D;
	}
}