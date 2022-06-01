#pragma once
#include <hzrpch.h>
#include "Hazard/Entity/World.h"

namespace Runtime {

	using RuntimeCallback = std::function<void(bool)>;

	class SceneRuntimeHandler {
	public:

		static void Init();

		static bool IsSceneRunning() { return s_SceneRunning; }
		static bool IsScenePaused() { return s_ScenePaused; }

		static void SetSceneRunning(bool running);
		static void SetScenePaused(bool paused);

		static void AddRuntimeCallback(RuntimeCallback callback) { m_Callbacks.push_back(callback); }

	private:
		static bool s_SceneRunning;
		static bool s_ScenePaused;

		static Ref<Hazard::ECS::World> s_EditorWorld;

		inline static std::vector<RuntimeCallback> m_Callbacks;
	};
}