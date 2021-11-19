#pragma once

#include "Hazard/Module.h"
#include "World.h"

namespace Hazard::ECS {
	enum Serialization { Runtime, Editor };

	class WorldHandler : public Module::Module {
	public:
		WorldHandler(EntityComponentCreateInfo* info);
		~WorldHandler();
		
		void Init() override;
		void Close() override;
		bool LoadWorld(const std::string& file, Serialization type = Serialization::Editor);

		Ref<World> GetCurrentWorld() { return m_World; }
		void SetWorld(Ref<World> world) { m_World = std::move(world); }

	private:
		Ref<World> m_World = nullptr;
	};
}