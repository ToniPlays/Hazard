#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Modules/Module.h"
#include "Hazard/Modules/Scene/Scene.h"

namespace Hazard {
	class SceneManager : public Module {

	public:
		SceneManager(Scene* scene);

		void LoadScene(std::string& path);
		void LoadEmptyScene();
		void ActivateScene(Scene* scene);

		Scene* GetActiveScene() { return activeScene; }
	private:
		Scene* activeScene;
	};
}