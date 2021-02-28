#pragma once

#include "Hazard/Module/Module.h"
#include "Scene.h"

namespace Hazard::ECS {

	enum Serialization { Runtime, Editor };

	class SceneHandler : public Module::Module {
	public:
		SceneHandler();
		~SceneHandler();

		void Init() override;
		void Update() override;
		void Render() override;

		bool LoadScene(const char* file, Serialization type = Serialization::Runtime);

		Scene& GetCurrentScene() { return *scene; }

	private:
		Scene* scene = nullptr;
	};
}