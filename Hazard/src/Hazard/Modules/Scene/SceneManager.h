#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Modules/Module.h"
#include "Hazard/Modules/Scene/Scene.h"

namespace Hazard {
	class SceneManager : public Module {

	public:
		SceneManager();
		SceneManager(Scene* scene);
		~SceneManager();

		void LoadScene(std::string& path);
		void LoadEmptyScene();
		void ActivateScene(Scene* scene);
		void OnDestroy() override;

		Scene* GetActiveScene() { return activeScene; }
	private:
		Scene* activeScene = nullptr;
	};
}