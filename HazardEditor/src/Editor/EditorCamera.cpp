
#include "EditorCamera.h"
#include "Backend/Input.h"
#include "MathCore.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Editor {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearPlane, float farPlane) : fov(fov), 
		aspectRatio(aspectRatio), m_Is2DEnabled(false)
	{
		m_ZNear = nearPlane;
		m_ZFar = farPlane;
		m_Projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		UpdateView();
	}
	void EditorCamera::OnUpdate()
	{
		using namespace HazardRenderer;
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
		return dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT(EditorCamera::OnMouseScroll));
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
		m_View = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
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
			xSpeed *= size2D * 2.0f;
			ySpeed *= size2D * 2.0f;
			m_FocalPoint += -GetRightDirection() * delta.x * xSpeed;
			m_FocalPoint += Math::GetUpDirection(GetOrientation()) * delta.y* ySpeed;
			return;
		}

		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += Math::GetUpDirection(GetOrientation()) * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = Math::GetUpDirection(GetOrientation()).y < 0 ? -1.0f : 1.0f;
		yaw += yawSign * delta.x * RotationSpeed();
		pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		if (m_Is2DEnabled) 
		{
			size2D -= delta * size2D * 0.25f;
			size2D = Math::Min(0.01f, size2D);
			SetIs2D(true);
			return;
		}
		
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
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
		float dist = m_Distance * 0.2f;
		dist = std::max(dist, 0.0f);
		float speed = dist * dist;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
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
			m_Projection = glm::ortho(-side, side, -size2D, size2D, -1000.0f, 1000.0f);

			if (enabled2D != m_Is2DEnabled) 
			{
				pitch = 0;
				yaw = 0;
			}
		}
		else 
			m_Projection = glm::perspective(glm::radians(fov), aspectRatio, m_ZNear, m_ZFar);

		m_Is2DEnabled = enabled2D;
	}
}
