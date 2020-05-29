#pragma once

#include "Hazard/Core/Core.h"
#include "GameObject.h"

namespace Hazard {
	class HAZARD_API Camera : public GameObject {
	public:
		Camera();
		static Transform GetCameraTransform();
		void SetSize(float size);
		float GetSize() { return size; };
	public:
		float size = 1.0f;
		static Camera* instance;
	};
}