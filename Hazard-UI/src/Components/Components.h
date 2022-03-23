#pragma once


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
	};

	struct Pivot 
	{	
		float PivotX = 0.5f;
		float PivotY = 0.5f;
	};
	struct RectTransform {
		Anchor Anchor;
		Pivot Pivot;
	};
}
