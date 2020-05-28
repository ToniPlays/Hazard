#pragma once

#include "Hazard/Core/Core.h"
#include <iostream>

namespace Hazard {
	class HAZARD_API File {
	public:
		static std::string ReadFile(std::string file);
	};
}