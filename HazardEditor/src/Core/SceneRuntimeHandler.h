#pragma once
#include "Hazard.h"

using namespace Hazard::Scripting;

namespace Runtime {

	class SceneRuntimeHandler {
	public:

		static void Init();

		static bool IsSceneRunning() { return s_SceneRunning; }
		static bool IsScenePaused() { return s_ScenePaused; }

		static void SetSceneRunning(bool running);
		static void SetScenePaused(bool paused);

	private:
		static bool s_SceneRunning;
		static bool s_ScenePaused;

		static ScriptEngineManager* s_ScriptManager;
		static ECS::World* s_EditorWorld;
	};
}