#pragma once

#include "Hazard/Module/Module.h"
#include "../World.h"

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
		World& GetCurrentWorld() { return *world; }

	private:
		World* world = nullptr;
	};
}