#pragma once

#include "Hazard/Utils/Maths/Vector/Vector3.h"
#include "Hazard/Utils/Maths/Vector/Vector2.h"
namespace Hazard {
	struct Vertex
	{
		Vector3<float> position;
		Vector3<float> normal;

		Vertex() = default;
		Vertex(Vector3<float> _position, Vector3<float> _normal) : position(_position), normal(_normal) {}
	};
}