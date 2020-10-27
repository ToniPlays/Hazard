#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/ECS/Components/Transform.h"
#include <ostream>
#include <iostream>
#include "glm/glm.hpp"
#include <glm/gtx/string_cast.hpp>

namespace Hazard {
	class HAZARD_API Matrix4 {
	public:

		static glm::mat4 GetModelMatrix(Transform transform);

		/*friend std::ostream& operator << (std::ostream& os, const glm::mat4& matrix) {
			os << glm::to_string(matrix);
			return os;
		}*/

		static void ToString(glm::mat4 matrix) {
			std::cout << glm::to_string(matrix) << std::endl;
		}
	};
}