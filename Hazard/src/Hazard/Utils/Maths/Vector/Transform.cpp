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

	Transform::Transform()
	{
		this->position = Vector3<float>::zero;
		this->rotation = Vector3<float>::zero;
		this->scale = Vector3<float>::one;
	}
}