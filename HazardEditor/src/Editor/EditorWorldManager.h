#pragma once

#include "Hazard.h"

namespace Editor 
{
	class EditorWorldManager {
	public:
		static void Init();
		static void Update();
		static void LoadWorld(AssetHandle handle);
		static void SetWorld(Ref<Hazard::World> world);
		static Ref<Hazard::WorldRenderer> GetWorldRender() { return s_WorldRenderer; }

	private:
		inline static Ref<Hazard::WorldRenderer> s_WorldRenderer;
	};
}