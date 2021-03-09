#pragma once

#include "Hazard/Module/Module.h"
#include "../Scene.h"

namespace Hazard::ECS {

	//Scene serialization type
	enum Serialization { Runtime, Editor };

	class SceneHandler : public Module::Module {
	public:
		SceneHandler();
		~SceneHandler();

		void Init() override;
		void Update() override;
		//Load scene from file and deserialize with specified type
		bool LoadScene(const char* file, Serialization type = Serialization::Runtime);
		//Get current scene
		Scene& GetCurrentScene() { return *scene; }

	private:
		Scene* scene = nullptr;
	};
}