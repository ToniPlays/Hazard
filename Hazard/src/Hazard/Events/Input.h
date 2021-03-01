#pragma once

#include "KeyCode.h"
#include "MouseCode.h"

namespace Hazard {

	class Input {
		public:
			static void Init();
			static bool IsKeyDown(const Key::KeyCode key);
			static bool IsKeyPressed(const Key::KeyCode key);
	};
}