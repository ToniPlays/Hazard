#pragma once
#include <hzreditor.h>
#include "SceneRuntimeHandler.h"
#include "GUI/EditorView.h"
#include "GUI/Window/AllWindows.h"

namespace Runtime {

	bool SceneRuntimeHandler::s_SceneRunning = false;
	bool SceneRuntimeHandler::s_ScenePaused = false;

	ScriptEngineManager* SceneRuntimeHandler::s_ScriptManager;

	void SceneRuntimeHandler::Init()
	{
		s_ScriptManager = &Application::GetModule<ScriptEngineManager>();
	}

	void SceneRuntimeHandler::SetSceneRunning(bool running)
	{
		s_SceneRunning = running;

		if (s_SceneRunning) {
			WindowElement::EditorView::SetWindowFocus<WindowElement::GameViewport>();
			ScriptCommand::OnBeginRuntime();
		}
		if (!s_ScenePaused) {
			WindowElement::EditorView::SetWindowFocus<WindowElement::Viewport>();
			ScriptCommand::OnEndRuntime(); 
		}
		if (s_ScenePaused && s_SceneRunning) return;
		s_ScriptManager->SetActive(s_SceneRunning);
	}
	void SceneRuntimeHandler::SetScenePaused(bool paused)
	{
		s_ScenePaused = paused;
		if (!s_SceneRunning) return;

		if (paused) {
			WindowElement::EditorView::SetWindowFocus<WindowElement::Viewport>();
		}
		else
			WindowElement::EditorView::SetWindowFocus<WindowElement::GameViewport>();
		s_ScriptManager->SetActive(!paused);
	}
}