#pragma once
#include "Hazard.h"

using namespace Hazard::Scripting;

namespace Runtime {

	class SceneRuntimeHandler {
	public:

		static void Init();

		static bool IsSceneRunning() { return sceneRunning; }
		static bool IsScenePaused() { return scenePaused; }

		static void SetSceneRunning(bool running);
		static void SetScenePaused(bool paused);

	private:
		static ScriptEngine* engine;

		static bool sceneRunning;
		static bool scenePaused;
	};
}