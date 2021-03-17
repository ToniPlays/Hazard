#pragma once
#include <hzreditor.h>
#include "SceneRuntimeHandler.h"

namespace Runtime {

	ScriptEngine* SceneRuntimeHandler::engine = nullptr;

	bool SceneRuntimeHandler::sceneRunning = false;
	bool SceneRuntimeHandler::scenePaused = false;


	void SceneRuntimeHandler::Init()
	{
		engine = &Application::GetModule<ScriptEngine>();
	}

	void SceneRuntimeHandler::SetSceneRunning(bool running)
	{
		sceneRunning = running;
		if (scenePaused && sceneRunning) return;
		if (sceneRunning) ScriptCommand::OnBeginRuntime();
		if (!sceneRunning) ScriptCommand::OnEndRuntime();
		engine->SetActive(sceneRunning);
	}
	void SceneRuntimeHandler::SetScenePaused(bool paused)
	{
		scenePaused = paused;
		if (!sceneRunning) return;
		engine->SetActive(!paused);
	}
}