#pragma once

#include <hzrpch.h>
#include "Vector3.h"

namespace Hazard {
	Vector3::Vector3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3::Vector3(float _x, float _y, float _z, Color* color)
	{
		x = _x;
		y = _y;
		z = _z;
		this->color = color;
	}
}