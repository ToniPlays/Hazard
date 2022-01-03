
#include "SceneRuntimeHandler.h"
#include "GUI/EditorView.h"
#include "GUI/Window/AllWindows.h"
#include "EditorEvent.h"
#include "GUI/EditorView.h"
#include "Core/EditorEvent.h"

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

			Hazard::Physics::PhysicsBeginInfo physicsInfo = {};

			Hazard::Physics::PhysicsCommand::BeginSimulation(&physicsInfo);
			WorldCommand::WorldRuntimeBegin();
			ScriptCommand::OnBeginRuntime();
		}
		else 
		{
			ScriptCommand::OnEndRuntime();
			WorldCommand::WorldRuntimeEnd();
			Hazard::Physics::PhysicsCommand::StopSimulation();
			handler.SetWorld(s_EditorWorld);
		}

		for (auto& callback : m_Callbacks) {
			callback(s_SceneRunning);
		}

		if (s_ScenePaused && s_SceneRunning) return;

		s_ScriptManager->SetActive(s_SceneRunning);
	}
	void SceneRuntimeHandler::SetScenePaused(bool paused)
	{
		s_ScenePaused = paused;

		if (!s_SceneRunning) 
			return;

		if (paused) {
			//WindowElement::EditorView::SetWindowFocus<WindowElement::Viewport>();
		}
		else {
			//WindowElement::EditorView::SetWindowFocus<WindowElement::GameViewport>();
		}
		s_ScriptManager->SetActive(!paused);
	}
}
