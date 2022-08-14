
#include "EditorModeManager.h"
#include "Core/GUIManager.h"
#include "GUI/AllPanels.h"
#include "Core/EditorEvent.h"

using namespace Hazard;

namespace Editor
{
	void EditorModeManager::BeginPlayMode()
	{
		s_CurrentMode = EditorMode::Play;
		auto& manager = Application::GetModule<GUIManager>();
		entt::entity currentEntity = manager.GetPanelManager().GetRenderable<UI::Viewport>()->GetSelectionContext().GetHandle();
		auto console = manager.GetPanelManager().GetRenderable<UI::Console>();

		if (console->ClearOnPlay())
		{
			console->Clear();
		}

		auto& handler = Application::GetModule<WorldHandler>();
		m_PreviousWorld = handler.GetCurrentWorld();
		Ref<World> playModeWorld = new World(*m_PreviousWorld);
		handler.SetWorld(playModeWorld);
		handler.SetFlags(WorldFlags_Render | WorldFlags_UpdateScript);
		handler.OnBegin();

		Events::SelectionContextChange e(Entity(currentEntity, playModeWorld.Raw()));
		manager.OnEvent(e);
	}
	void EditorModeManager::EndPlayMode()
	{
		s_CurrentMode = EditorMode::Edit;
		auto& handler = Application::GetModule<WorldHandler>();
		handler.OnEnd();
		handler.SetFlags(WorldFlags_Render);
		handler.SetWorld(m_PreviousWorld);


		auto& manager = Application::GetModule<GUIManager>();
		entt::entity currentEntity = manager.GetPanelManager().GetRenderable<UI::Viewport>()->GetSelectionContext().GetHandle();
		Events::SelectionContextChange e({ currentEntity, m_PreviousWorld.Raw() });
		manager.OnEvent(e);
	}
}
