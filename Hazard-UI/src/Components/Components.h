#pragma once

#include "Color.h"

namespace HazardUI 
{
	struct Anchor {
		float MinX = 0.5f;
		float MinY = 0.5f;
		float MaxX = 0.5f;
		float MaxY = 0.5f;

		Anchor() {};

		Anchor(float minX, float minY, float maxX, float maxY) : MinX(minX),
			MinY(minY), MaxX(maxX), MaxY(maxY) {}

		bool operator == (const Anchor& f) {
			return MinX == f.MinX && MaxX == f.MaxX && MinY == f.MinY && MaxY == f.MaxY;
		}

		static Anchor TopLeft;
		static Anchor TopCenter;
		static Anchor TopRight;

		static Anchor MiddleLeft;
		static Anchor MiddleCenter;
		static Anchor MiddleRight;

		static Anchor BottomLeft;
		static Anchor BottomCenter;
		static Anchor BottomRight;

		static Anchor FullStretch;
	};

	struct Pivot 
	{	
		float PivotX = 0.5f;
		float PivotY = 0.5f;
	};

	struct RectTransform {
		Anchor Anchor;
		Pivot Pivot;

		float Left;
		float Right;
		float Top;
		float Bottom;

		float Width;
		float Height;

		//Temp
		Color _color;
	};
}
