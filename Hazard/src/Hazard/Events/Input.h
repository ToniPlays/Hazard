#pragma once

#include "KeyCode.h"
#include "MouseCode.h"
#include <glm/glm.hpp>

namespace Hazard {

	class Input {
		public:
			static void Init();
			static bool IsKeyDown(const Key::KeyCode key);
			static bool IsKeyPressed(const Key::KeyCode key);

			static bool IsMouseButtonDown(const Mouse::MouseCode code);
			static bool IsMouseButtonPressed(const Mouse::MouseCode code);

			static glm::vec2 GetMousePos();
	};
}