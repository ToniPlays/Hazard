#pragma once

#include <hzrpch.h>
#include "Camera.h"

namespace Hazard {
	
	Camera* Camera::instance;

	Camera::Camera() : GameObject("Camera")
	{
		Camera::instance = this;
	}

	Transform Camera::GetCameraTransform()
	{
		return instance->transform;
	}

}