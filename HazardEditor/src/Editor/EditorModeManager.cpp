
#include "EditorModeManager.h"
#include "GUI/GUIManager.h"
#include "GUI/AllPanels.h"
#include "Core/EditorEvent.h"
#include "EditorWorldManager.h"

using namespace Hazard;

namespace Editor
{
	void EditorModeManager::BeginPlayMode()
	{
		if (s_CurrentMode == EditorMode::Play) return;

		s_CurrentMode = EditorMode::Play;
		HZR_WARN("BeginPlayMode");
		auto& manager = Application::GetModule<GUIManager>();
		//entt::entity currentEntity = manager.GetPanelManager().GetRenderable<UI::Viewport>()->GetSelectionContext().GetHandle();
		auto console = manager.GetPanelManager().GetRenderable<UI::Console>();

		if (console->ClearOnPlay())
			console->Clear();

		auto& handler = Application::GetModule<WorldHandler>();
		m_PreviousWorld = handler.GetCurrentWorld();
		Ref<World> playModeWorld = World::Copy(m_PreviousWorld);

		Editor::EditorWorldManager::GetWorldRender()->SetTargetWorld(playModeWorld);

		handler.SetWorld(playModeWorld);
		handler.SetFlags(WorldFlags_Render | WorldFlags_UpdateScript);
		handler.OnBegin();

		Events::SelectionContextChange e({});
		Hazard::HazardLoop::GetCurrent().OnEvent(e);
	}
	void EditorModeManager::EndPlayMode()
	{
		if (s_CurrentMode != EditorMode::Play) return;

		HZR_WARN("StopPlayMode");
		s_CurrentMode = EditorMode::Edit;
		auto& handler = Application::GetModule<WorldHandler>();
		handler.OnEnd();
		handler.SetFlags(WorldFlags_Render);
		handler.SetWorld(m_PreviousWorld);
		Editor::EditorWorldManager::GetWorldRender()->SetTargetWorld(m_PreviousWorld);

		Events::SelectionContextChange e({});
		Hazard::HazardLoop::GetCurrent().OnEvent(e);
	}
}
