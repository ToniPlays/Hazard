#pragma once
#include "Core/Core.h"
#include "nlohmann/json.hpp"
#include <iostream>

using json = nlohmann::json;

namespace Hazard {
	class HAZARD_API JSON {
	public:
		static json ReadFile(std::string file);
	};
}