#pragma once

#include "SceneRuntimeHandler.h"
#include "GUI/EditorView.h"
#include "GUI/Window/AllWindows.h"
#include "EditorEvent.h"
#include "GUI/EditorView.h"

namespace Runtime {

	bool SceneRuntimeHandler::s_SceneRunning = false;
	bool SceneRuntimeHandler::s_ScenePaused = false;
	Ref<ECS::World> SceneRuntimeHandler::s_EditorWorld = nullptr;

	ScriptEngineManager* SceneRuntimeHandler::s_ScriptManager;

	void SceneRuntimeHandler::Init()
	{
		s_ScriptManager = &Application::GetModule<ScriptEngineManager>();
	}

	void SceneRuntimeHandler::SetSceneRunning(bool running)
	{
		s_SceneRunning = running;

		ECS::WorldHandler& handler = Application::GetModule<ECS::WorldHandler>();

		if (s_SceneRunning) 
		{
			s_EditorWorld = handler.GetCurrentWorld();
			World* runtimeWorld = new World(*s_EditorWorld);
			handler.SetWorld(runtimeWorld);

			PhysicsCommand::OnBeginRuntime(-9.8f, runtimeWorld);
			ScriptCommand::OnBeginRuntime();
		}
		else 
		{
			PhysicsCommand::OnEndRuntime();
			ScriptCommand::OnEndRuntime();

			delete &handler.GetCurrentWorld();
			handler.SetWorld(s_EditorWorld);
		}
		if (s_ScenePaused && s_SceneRunning) 
			return;
		s_ScriptManager->SetActive(s_SceneRunning);
	}
	void SceneRuntimeHandler::SetScenePaused(bool paused)
	{
		s_ScenePaused = paused;

		if (!s_SceneRunning) 
			return;

		if (paused) {
			WindowElement::EditorView::SetWindowFocus<WindowElement::Viewport>();
		}
		else
			WindowElement::EditorView::SetWindowFocus<WindowElement::GameViewport>();
		s_ScriptManager->SetActive(!paused);
	}
}