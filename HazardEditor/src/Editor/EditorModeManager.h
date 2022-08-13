#pragma once

#include "Hazard.h"

namespace Editor 
{
	enum class EditorMode {
		Edit = 0,
		Play,
		Simulate
	};

	class EditorModeManager 
	{
	public:
		static void BeginPlayMode();
		static void EndPlayMode();
		static EditorMode GetCurrentMode() { return s_CurrentMode; }
	private:
		inline static Ref<Hazard::World> m_PreviousWorld;
		inline static EditorMode s_CurrentMode;
	};
}