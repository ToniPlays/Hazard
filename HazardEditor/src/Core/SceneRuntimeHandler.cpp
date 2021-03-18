#pragma once
#include <hzreditor.h>
#include "SceneRuntimeHandler.h"
#include "GUI/EditorView.h"
#include "GUI/Window/AllWindows.h"

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

		if (sceneRunning) {
			WindowElement::EditorView::SetWindowFocus<WindowElement::GameViewport>();
			ScriptCommand::OnBeginRuntime();
		}
		if (!sceneRunning) { 
			WindowElement::EditorView::SetWindowFocus<WindowElement::Viewport>();
			ScriptCommand::OnEndRuntime(); 
		}
		if (scenePaused && sceneRunning) return;
		engine->SetActive(sceneRunning);
	}
	void SceneRuntimeHandler::SetScenePaused(bool paused)
	{
		scenePaused = paused;
		if (!sceneRunning) return;

		if (paused) {
			WindowElement::EditorView::SetWindowFocus<WindowElement::Viewport>();
		}
		else
			WindowElement::EditorView::SetWindowFocus<WindowElement::GameViewport>();
		engine->SetActive(!paused);
	}
}