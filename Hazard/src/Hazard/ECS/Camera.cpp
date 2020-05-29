#pragma once

#include <hzrpch.h>
#include "Camera.h"

namespace Hazard {
	
	Camera* Camera::instance;

	Camera::Camera() : GameObject("Camera", Transform())
	{
		Camera::instance = this;
	}

	Transform Camera::GetCameraTransform()
	{
		return instance->transform;
	}

	void Camera::SetSize(float size)
	{
		this->size = size;
		this->transform.scale.SetAll(size);
	}
}