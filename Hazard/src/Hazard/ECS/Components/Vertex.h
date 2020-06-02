#pragma once

#include "Hazard/Utils/Maths/Vector/Vector3.h"
#include "Hazard/Utils/Maths/Vector/Vector2.h"
namespace Hazard {
	struct Vertex
	{
		Vector3<float> position;
		Vector3<float> normal;
		Vector2<float> textureCoords;

		Vertex() = default;
		Vertex(Vector3<float> _position, Vector2<float> _textureCoords, Vector3<float> _normal) : position(_position),
			textureCoords(_textureCoords), normal(_normal)
		{
		}
	};
}