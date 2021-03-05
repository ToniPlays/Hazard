#pragma once

#include "entt.hpp"
#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard::ECS {
	class SceneCommand {
	public:

		static void Init();

		template<typename C, typename T>
		static void Render(C& component, T& transform);

	private:
		static Rendering::RenderEngine engine;
	};
}