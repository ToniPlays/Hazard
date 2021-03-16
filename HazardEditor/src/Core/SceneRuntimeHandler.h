#pragma once
#include "Hazard.h"

namespace Runtime {

	class SceneRuntimeHandler {
	public:
		static bool IsSceneRunning() { return sceneRunning; }
		static bool IsScenePaused() { return scenePaused; }

		static void SetSceneRunning(bool running) { sceneRunning = running; }
		static void SetScenePaused(bool paused) { scenePaused = paused; }
	private:
		static bool sceneRunning;
		static bool scenePaused;
	};
}