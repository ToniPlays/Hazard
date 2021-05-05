#pragma once

#include "Hazard/Module/Module.h"
#include "../World.h"

namespace Hazard::ECS {
	enum Serialization { Runtime, Editor };

	class SceneHandler : public Module::Module {
	public:
		SceneHandler();
		~SceneHandler();
		bool LoadScene(const char* file, Serialization type = Serialization::Runtime);

		World& GetCurrentWorld() { return *m_World; }
		void SetWorld(World& world) { m_World = &world; }

	private:
		World* m_World = nullptr;
	};
}