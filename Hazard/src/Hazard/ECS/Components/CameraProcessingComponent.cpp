
#include "hzrpch.h"
#include "CameraProcessingComponent.h"
#include "MathCore.h"

namespace Hazard {

	void CameraComponent::SetProjection(Projection t)
	{
		m_Type = t;
		RecalculateProjection(m_Width, m_Height);
	}
	void CameraComponent::SetFov(float fov)
	{
		m_Dirty |= m_Fov == fov;
		m_Fov = fov;
		RecalculateProjection(m_Width, m_Height);
	}
	void CameraComponent::SetSize(float size)
	{
		m_Dirty |= m_Size == size;
		m_Size = size;
		RecalculateProjection(m_Width, m_Height);
	}
	void CameraComponent::SetZNear(float plane)
	{
		m_Dirty |= m_ZNear == plane;
		m_ZNear = plane;
		RecalculateProjection(m_Width, m_Height);
	}
	void CameraComponent::SetZFar(float plane)
	{
		m_Dirty |= m_ZFar == plane;
		m_ZFar = plane;
		RecalculateProjection(m_Width, m_Height);
	}
	void CameraComponent::RecalculateProjection(float w, float h)
	{
		if (!m_Dirty) return;

		m_Width = w;
		m_Height = h;

		float aspectRatio = w / h;

		if (m_Type == Projection::Perspective) 
		{
			m_Projection = glm::perspective(glm::radians(m_Fov), aspectRatio, Math::Min(0.003f, m_ZNear), m_ZFar);
			return;
		}

		float orthoLeft = -m_Size * aspectRatio * 0.5f;
		float orthoRight = m_Size * aspectRatio * 0.5f;
		float orthoBottom = -m_Size * 0.5f;
		float orthoTop = m_Size * 0.5f;

		m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_ZNear, m_ZFar);
	}
}
