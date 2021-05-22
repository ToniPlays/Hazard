#pragma once

#include "hzrpch.h"
#include "CameraProcessingComponent.h"

namespace Hazard::ECS {

	void CameraComponent::SetProjection(Projection t)
	{
		m_Type = t;
		RecalculateProjection(m_Width, m_Height);
	}
	void CameraComponent::SetFov(float fov)
	{
		this->m_Fov = fov;
		RecalculateProjection(m_Width, m_Height);
	}
	void CameraComponent::SetZNear(float plane)
	{
		m_ZNear = plane;
		RecalculateProjection(m_Width, m_Height);
	}
	void CameraComponent::SetZFar(float plane)
	{
		m_ZFar = plane;
		RecalculateProjection(m_Width, m_Height);
	}
	void CameraComponent::RecalculateProjection(float w, float h)
	{
		m_Width = w;
		m_Height = h;

		float aspectRatio = w / h;

		if (m_Type == Perspective) {
			m_Projection = glm::perspective(glm::radians(m_Fov), aspectRatio, Math::Max(0.003f, m_ZNear), m_ZFar);
		}
		else
		{
			float orthoLeft = -m_Fov * aspectRatio * 0.5f;
			float orthoRight = m_Fov * aspectRatio * 0.5f;
			float orthoBottom = -m_Fov * 0.5f;
			float orthoTop = m_Fov * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_ZNear, m_ZFar);
		}
	}
}
