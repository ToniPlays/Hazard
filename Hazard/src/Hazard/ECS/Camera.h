#pragma once

#include "Hazard/Core/Core.h"
#include "GameObject.h"

namespace Hazard {
	class HAZARD_API Camera : public GameObject {
	public:
		Camera();
		static Transform GetCameraTransform();
	public:
		static Camera* instance;
	};
}