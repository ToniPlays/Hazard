#pragma once

#include <hzrpch.h>
#include "Transform.h"

namespace Hazard {

	Transform::Transform(Vector3<float> position, Vector3<float> rotation, Vector3<float> scale)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
}