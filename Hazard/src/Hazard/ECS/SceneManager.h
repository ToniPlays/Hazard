#pragma once

#include "Hazard/Core/Core.h"
#include "Scene.h"
#include "Hazard/Module.h"

namespace Hazard {
	namespace ECS {
		class SceneManager : public Module {

		public:
			SceneManager();

			Scene& GetActiveScene() { return *scene; }

		private:
			Scene* scene;
		};
	}
}