
#include "Components.h"


namespace HazardUI {
	//							Left: 0, Right 1, Top: 1, Bottom 0
	Anchor Anchor::TopLeft		= { 0.0f, 1.0f, 0.0f, 1.0f };
	Anchor Anchor::TopCenter	= { 0.5f, 1.0f, 0.5f, 1.0f };
	Anchor Anchor::TopRight		= { 1.0f, 1.0f, 1.0f, 1.0f };

	Anchor Anchor::MiddleLeft	= { 0.0f, 0.5f, 0.0f, 0.5f };
	Anchor Anchor::MiddleCenter	= { 0.5f, 0.5f, 0.5f, 0.5f };
	Anchor Anchor::MiddleRight	= { 1.0f, 0.5f, 1.0f, 0.5f };

	Anchor Anchor::BottomLeft	= { 0.0f, 0.0f, 0.0f, 0.0f };
	Anchor Anchor::BottomCenter = { 0.5f, 0.0f, 0.5f, 0.0f };
	Anchor Anchor::BottomRight	= { 1.0f, 0.0f, 1.0f, 0.0f };

	Anchor Anchor::FullStretch	= { 0.0f, 0.0f, 1.0f, 1.0f };
}