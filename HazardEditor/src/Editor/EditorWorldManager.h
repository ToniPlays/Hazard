#pragma once

#include "Hazard.h"

namespace Editor 
{
	class EditorWorldManager {
	public:
		static void Init();
		static void Update();
		static Ref<Hazard::WorldRenderer> GetWorldRender() { return m_WorldRenderer; }

	private:
		inline static Ref<Hazard::WorldRenderer> m_WorldRenderer;
	};
}