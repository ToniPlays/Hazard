#pragma once

#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Hazard/Module.h"
#include "World.h"

namespace Hazard 
{
	enum Serialization { Runtime, Editor };

	class WorldHandler : public Module {
	public:
		WorldHandler(EntityComponentCreateInfo* info);
		~WorldHandler();
		
		void Init() override;
		void Close() override;
		void Update() override;
		bool LoadWorld(const std::string& file, Serialization type = Serialization::Editor);

		Ref<World> GetCurrentWorld() { return m_World; }
		void SetWorld(Ref<World> world) { m_World = std::move(world); }

	private:
		Ref<World> m_World = nullptr;
		std::vector<Ref<World>> m_LoadedWorlds;
	};
}