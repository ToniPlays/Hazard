#pragma once
#include "Hazard/Utils/Maths/Vector/Vector2.h"
#include "Hazard/Utils/Maths/Vector/Vector3.h"
#include "Hazard/Utils/Maths/Color.h"

namespace Hazard {

	struct Vertex {
		Vector3<float> position;
		Color color;
	};
}