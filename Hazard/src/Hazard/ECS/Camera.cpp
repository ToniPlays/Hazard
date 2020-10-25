#pragma once

#include <hzrpch.h>
#include "Camera.h"

namespace Hazard {


	Camera::Camera() : Entity("Camera")
	{
		
	}

	Transform Camera::GetTransform()
	{
		return Transform();
	}

}