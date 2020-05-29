#pragma once

#include "Hazard/Core/Core.h"
#include "Vector3.h"

namespace Hazard {
	class HAZARD_API Transform {

	public:
		Transform(Vector3<float> position = Vector3<float>::zero, Vector3<float> rotation = Vector3<float>::zero, Vector3<float> scale = Vector3<float>::one);
	public:
		Vector3<float> position, rotation, scale;
	};
}