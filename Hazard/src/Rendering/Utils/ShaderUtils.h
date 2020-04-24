#pragma once

#include "Core/Core.h"
#include <iostream>

namespace Hazard {
	class HAZARD_API ShaderUtils {
	public:
		static void LoadShader(std::string vertex, std::string fragment);
	};
}