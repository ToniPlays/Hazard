
#include "EditorModeManager.h"

using namespace Hazard;

namespace Editor 
{
	void EditorModeManager::BeginPlayMode()
	{
		HZR_WARN("Begining play mode");
		s_CurrentMode = EditorMode::Play;
		auto& handler = Application::GetModule<WorldHandler>();
		m_PreviousWorld = handler.GetCurrentWorld();
		Ref<World> playModeWorld = new World(*m_PreviousWorld);
		handler.SetWorld(playModeWorld);
		handler.SetFlags(WorldFlags_Render | WorldFlags_UpdateScript);
		handler.OnBegin();
	}
	void EditorModeManager::EndPlayMode()
	{
		HZR_CORE_WARN("Ending play mode");
		s_CurrentMode = EditorMode::Edit;
		auto& handler = Application::GetModule<WorldHandler>();
		handler.OnEnd();
		handler.SetFlags(WorldFlags_Render);
		handler.SetWorld(m_PreviousWorld);
	}
}
