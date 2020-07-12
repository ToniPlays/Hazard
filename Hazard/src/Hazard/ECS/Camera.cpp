#pragma once

#include <hzrpch.h>
#include "Camera.h"

namespace Hazard {

	Camera* Camera::instance;

	Camera::Camera() : GameObject("Camera", Transform())
	{
		Camera::instance = this;
	}

	Transform Camera::GetTransform()
	{
		Transform t = instance->transform;
		float s = instance->size;
		return Transform(Vector3<float>::Invert(t.position), Vector3<float>::Invert(t.rotation), Vector3<float>::Invert(t.scale) / -s);
	}

	void Camera::SetSize(float size)
	{
		this->size = size;
	}
}