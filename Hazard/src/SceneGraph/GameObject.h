#pragma once

#include "Core/Core.h"

namespace Hazard {
	class HAZARD_API GameObject {
		public:
			GameObject(std::string _name) : name(_name) {}
			std::string name = "GameObject";
	};
}