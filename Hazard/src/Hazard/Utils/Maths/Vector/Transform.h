#pragma once

#include "Hazard/Core/Core.h"
#include "Vector3.h"

namespace Hazard {
	class HAZARD_API Transform {

	public:
		Transform();
		Transform(Vector3<float> position, Vector3<float> rotation, Vector3<float> scalee);
	public:
		Vector3<float> position, rotation, scale;
	};
}