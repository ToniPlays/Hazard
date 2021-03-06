#pragma once

#include "Hazard/Module.h"
#include "World.h"

namespace Hazard::ECS {
	enum Serialization { Runtime, Editor };

	class WorldHandler : public Module::Module {
	public:
		WorldHandler();
		~WorldHandler();
		bool LoadWorld(const std::string& file, Serialization type = Serialization::Editor);

		World& GetCurrentWorld() { return *m_World.Raw(); }
		void SetWorld(World& world) { m_World = Ref<World>(&world); }

	private:
		Ref<World> m_World = nullptr;
	};
}