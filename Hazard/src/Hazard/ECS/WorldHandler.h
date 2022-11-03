#pragma once

#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Hazard/Module.h"
#include "World.h"

namespace Hazard 
{
	enum Serialization { Runtime, Editor };
	enum WorldFlags : uint32_t {
		WorldFlags_Render = BIT(0),
		WorldFlags_UpdateScript = BIT(1)
	};

	class WorldHandler : public Module {
	public:
		WorldHandler(EntityComponentCreateInfo* info);
		~WorldHandler();
		
		void Init() override;
		void Close() override;
		void Update() override;

		void OnBegin();
		void OnEnd();

		bool LoadWorld(const std::filesystem::path& file, Serialization type = Serialization::Editor);
		void SetFlags(uint32_t flags) { m_Flags = (WorldFlags)flags; }
		Ref<World> GetCurrentWorld() { return m_World; }
		void SetWorld(Ref<World> world) { m_World = world; }

	private:
		Ref<World> m_World = nullptr;
		std::vector<Ref<World>> m_LoadedWorlds;
		WorldFlags m_Flags = WorldFlags_Render;
	};
}