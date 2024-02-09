#include "EditorWorldManager.h"
#include "Hazard/ECS/WorldHandler.h"
#include "Core/GUIManager.h"
#include "GUI/Overlays/ProgressOverlay.h"
#include "Core/EditorEvent.h"
#include "Core/HazardEditor.h"

namespace Editor
{
	using namespace Hazard;

	void EditorWorldManager::Init()
	{
		WorldRendererSpec spec = {};
		spec.Geometry = Geometry_All;
		spec.DebugName = "MainWorld";
		spec.TargetWorld = Application::GetModule<WorldHandler>().GetCurrentWorld();
		s_WorldRenderer = Ref<WorldRenderer>::Create(&spec);
	}
	void EditorWorldManager::Update()
	{
		HZR_PROFILE_FUNCTION();
		s_WorldRenderer->Submit();
	}
	void EditorWorldManager::LoadWorld(AssetHandle handle)
	{
		JobPromise promise = AssetManager::GetAssetAsync(handle);
		promise.Then([](JobGraph& graph) {
			Ref<World> result = graph.GetResult<Ref<World>>();
			if (!result) return;

			auto& handler = Application::GetModule<WorldHandler>();
			handler.SetWorld(result);
			s_WorldRenderer->SetTargetWorld(handler.GetCurrentWorld());

			Events::SelectionContextChange e({});
			HazardLoop::GetCurrent().OnEvent(e);
		});
	}
}
