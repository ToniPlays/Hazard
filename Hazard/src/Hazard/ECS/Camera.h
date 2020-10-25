#pragma once

#include "Hazard/Core/Core.h"
#include "Entity.h"
#include "Hazard/Utils/Maths/Vector/Transform.h"

namespace Hazard {
	class HAZARD_API Camera : public Entity {
	public:
		Camera();
		static Transform GetTransform();
	};
}