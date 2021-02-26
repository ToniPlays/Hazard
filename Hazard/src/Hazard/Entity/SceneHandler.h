#pragma once

#include "Hazard/Module/Module.h"
#include "Scene.h"

namespace Hazard::ECS {
	class SceneHandler : public Module::Module {
	public:
		SceneHandler();
		~SceneHandler();

		void Init() override;
		void Update() override;
		void Render() override;

		void LoadScene(const char* file);

		Scene& GetCurrentScene() { return *scene; }

	private:
		Scene* scene = nullptr;
	};
}