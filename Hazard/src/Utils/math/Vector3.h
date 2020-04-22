#pragma once
#include "Utils/Color.h"

namespace Hazard {

	class Vector3 {
	public:
		float x, y, z;
		Color* color = nullptr;

		Vector3() : x(0), y(0), z(0) {}
		Vector3(float _x, float _y, float _z);
		Vector3(float _x, float _y, float _z, Color* color);

	};
}