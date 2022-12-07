
#include "EditorCamera.h"
#include "Backend/Input.h"
#include "MathCore.h"
#include "Hazard/Math/Time.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Editor {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearPlane, float farPlane) : fov(fov),
		aspectRatio(aspectRatio), m_Is2DEnabled(false), m_ZNear(nearPlane), m_ZFar(farPlane), m_FocalPoint(0.0f)
	{
		m_Projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		Init();
	}
	void EditorCamera::Init()
	{
		constexpr glm::vec3 position = { -5, 5, 5 };
		m_Distance = glm::distance(position, m_FocalPoint);

		m_Yaw = 3.0f * glm::pi<float>() / 4.0f;
		m_Pitch = glm::pi<float>() / 4.0f;

		m_Position = CalculatePosition();
		const glm::quat orientation = GetOrientation();
		m_Direction = glm::eulerAngles(orientation) * (180.0f / glm::pi<float>());
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}
	void EditorCamera::OnUpdate()
	{
		const glm::vec2& mouse = Input::GetMousePos();
		const glm::vec2 delta = (mouse - m_InitialMousePos) * 0.002f;

		if (Input::IsMouseButtonDown(Mouse::ButtonRight) && !Input::IsKeyDown(Key::LeftAlt))
		{
			m_CameraMode = CameraMode::FlyCam;

			const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
			const float speed = GetCameraSpeed();

			if (Input::IsKeyDown(Key::Q))
				m_PositionDelta -= (float)Time::s_UnscaledDeltaTime * speed * glm::vec3(0.0f, yawSign, 0.0f);
			if (Input::IsKeyDown(Key::E))
				m_PositionDelta += (float)Time::s_UnscaledDeltaTime * speed * glm::vec3(0.0f, yawSign, 0.0f);
			if (Input::IsKeyDown(Key::S))
				m_PositionDelta -= (float)Time::s_UnscaledDeltaTime * speed * m_Direction;
			if (Input::IsKeyDown(Key::W))
				m_PositionDelta += (float)Time::s_UnscaledDeltaTime * speed * m_Direction;
			if (Input::IsKeyDown(Key::A))
				m_PositionDelta -= (float)Time::s_UnscaledDeltaTime * speed * m_RightDirection;
			if (Input::IsKeyDown(Key::D))
				m_PositionDelta += (float)Time::s_UnscaledDeltaTime * speed * m_RightDirection;

			constexpr float maxRate = 0.12f;
			m_YawDelta += glm::clamp(yawSign * delta.x * RotationSpeed(), -maxRate, maxRate);
			m_PitchDelta += glm::clamp(delta.y * RotationSpeed(), -maxRate, maxRate);

			m_RightDirection = glm::cross(m_Direction, glm::vec3(0.0f, yawSign, 0.0f));
			
			m_Direction = glm::rotate(glm::normalize(glm::cross(glm::angleAxis(-m_PitchDelta, m_RightDirection),
				glm::angleAxis(-m_YawDelta, glm::vec3(0.0f, yawSign, 0.0f)))), m_Direction);

			const float distance = glm::distance(m_FocalPoint, m_Position);
			m_FocalPoint = m_Position + GetForwardDirection() * distance;
			m_Distance = distance;
		}
		else if (Input::IsKeyDown(Key::LeftAlt))
		{
			m_CameraMode = CameraMode::Arcball;
			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
			{
				MousePan(delta);
			}
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
			{
				MouseRotate(delta);
			}
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
			{
				MouseZoom(delta.x + delta.y);
			}
			else
			{
				//Enable mouse
			}
		}

		m_InitialMousePos = mouse;
		m_Position += m_PositionDelta;
		m_Yaw += m_YawDelta;
		m_Pitch += m_PitchDelta;

		if (m_CameraMode == CameraMode::Arcball)
		{
			m_Position = CalculatePosition();
		}
		UpdateView();
	}

	bool EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT(EditorCamera::OnMouseScroll));
	}
	float EditorCamera::GetCameraSpeed() const
	{
		float speed = m_NormalSpeed;

		if (Input::IsKeyDown(Key::LeftShift))
			speed /= 2 - glm::log(m_NormalSpeed);
		if (Input::IsKeyDown(Key::LeftControl))
			speed *= 2 - glm::log(m_NormalSpeed);
		return glm::clamp(speed, MIN_SPEED, MAX_SPEED) * m_SpeedMultiplier;
	}

	void EditorCamera::UpdateProjection()
	{
		aspectRatio = viewport_w / viewport_h;
		SetIs2D(m_Is2DEnabled);
	}

	void EditorCamera::UpdateView()
	{
		const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;

		const float cosAngle = glm::dot(GetForwardDirection(), GetUpDirection());
		if (cosAngle * yawSign > 0.99f)
			m_PitchDelta = 0.0f;

		const glm::vec3 lookAt = m_Position + GetForwardDirection();
		m_Direction = glm::normalize(lookAt - m_Position);
		m_Distance = glm::distance(m_Position, m_FocalPoint);
		m_ViewMatrix = glm::lookAt(m_Position, lookAt, glm::vec3(0.0f, yawSign, 0.0f));

		m_YawDelta *= 0.6f;
		m_PitchDelta *= 0.6f;
		m_PositionDelta *= 0.8f;
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

		if (m_Is2DEnabled) 
		{
			xSpeed *= size2D * 2.0f;
			ySpeed *= size2D * 2.0f;
			m_FocalPoint += -GetRightDirection() * delta.x * xSpeed;
			m_FocalPoint += Math::GetUpDirection(GetOrientation()) * delta.y * ySpeed;
			return;
		}

		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += Math::GetUpDirection(GetOrientation()) * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		/*
		if (m_Is2DEnabled)
		{
			size2D -= delta * size2D * 0.25f;
			size2D = Math::Min(0.01f, size2D);
			SetIs2D(true);
			return;
		}
		*/

		m_Distance -= delta * ZoomSpeed();
		const glm::vec3 forwardDir = GetForwardDirection();
		m_Position = m_FocalPoint - forwardDir * m_Distance;
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += forwardDir * m_Distance;
			m_Distance = 1.0f;
		}
		m_PositionDelta += delta * ZoomSpeed() * forwardDir;
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
		return 0.3f;
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
		return m_FocalPoint - GetForwardDirection() * m_Distance + m_PositionDelta;
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
		return glm::quat(glm::vec3(-m_Pitch - m_PitchDelta, -m_Yaw - m_YawDelta, 0.0f));
	}
	void EditorCamera::SetIs2D(bool enabled2D)
	{
		if (enabled2D)
		{
			float side = aspectRatio * size2D;
			m_Projection = glm::ortho(-side, side, -size2D, size2D, -1000.0f, 1000.0f);

			if (enabled2D != m_Is2DEnabled)
			{
				m_Pitch = 0;
				m_Yaw = 0;
			}
		}
		else
			m_Projection = glm::perspective(glm::radians(fov), aspectRatio, m_ZNear, m_ZFar);

		m_Is2DEnabled = enabled2D;
	}
}
