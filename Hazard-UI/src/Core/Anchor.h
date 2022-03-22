#pragma once

namespace HazardUI 
{
	struct Anchor 
	{
		float TopLeft = 0.5f;
		float TopRight = 0.5f;
		float BottomLeft = 0.5f;
		float BottomRight = 0.5f;

		Anchor() {};
		Anchor(float topLeft, float topRight, float bottomLeft, float bottomRight) : TopLeft(topLeft), 
			TopRight(topRight), BottomLeft(bottomLeft), BottomRight(BottomRight) {}
	};
}