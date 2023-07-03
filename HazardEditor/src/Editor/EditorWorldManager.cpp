#include "EditorWorldManager.h"
#include "Hazard/ECS/WorldHandler.h"
#include "Core/GUIManager.h"
#include "GUI/Overlays/ProgressOverlay.h"
#include "Core/EditorEvent.h"

namespace Editor 
{
	using namespace Hazard;

	void EditorWorldManager::Init()
	{
		WorldRendererSpec spec = {};
		spec.Geometry = Geometry_All;
		spec.DebugName = "MainWorld";
		s_WorldRenderer = Ref<WorldRenderer>::Create(&spec);
	}
	void EditorWorldManager::Update()
	{
		HZR_PROFILE_FUNCTION();
		s_WorldRenderer->Submit();
	}
	void EditorWorldManager::LoadWorld(AssetHandle handle)
	{
		auto& handler = Application::GetModule<WorldHandler>();
		handler.LoadWorld(handle);
		s_WorldRenderer->SetTargetWorld(handler.GetCurrentWorld());

        Events::SelectionContextChange e({});
        HazardLoop::GetCurrent().OnEvent(e);
	}
}
