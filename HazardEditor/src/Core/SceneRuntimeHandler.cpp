#pragma once
#include <hzreditor.h>
#include "SceneRuntimeHandler.h"
#include "GUI/EditorView.h"
#include "GUI/Window/AllWindows.h"
#include "EditorEvent.h"
#include "GUI/EditorView.h"

namespace Runtime {

	bool SceneRuntimeHandler::s_SceneRunning = false;
	bool SceneRuntimeHandler::s_ScenePaused = false;
	ECS::World* SceneRuntimeHandler::s_EditorWorld = nullptr;

	ScriptEngineManager* SceneRuntimeHandler::s_ScriptManager;

	void SceneRuntimeHandler::Init()
	{
		s_ScriptManager = Application::GetModule<ScriptEngineManager>();
	}

	void SceneRuntimeHandler::SetSceneRunning(bool running)
	{
		s_SceneRunning = running;
		ECS::SceneHandler& handler = *Application::GetModule<ECS::SceneHandler>();

		Events::SelectionContextChange e ({});
		WindowElement::EditorView::GetInstance().OnEvent(e);

		if (s_SceneRunning) {

			s_EditorWorld = &handler.GetCurrentWorld();
			World* runtimeWorld = new World(*s_EditorWorld);
			handler.SetWorld(*runtimeWorld);

			ScriptCommand::OnBeginRuntime();
		}
		else 
		{
			delete &handler.GetCurrentWorld();
			handler.SetWorld(*s_EditorWorld);
		}

		if (!s_ScenePaused) {
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