#include "EditorWorldManager.h"
#include "Hazard/ECS/WorldHandler.h"
#include "GUI/Overlays/ProgressOverlay.h"
#include "Core/EditorEvent.h"
#include "Core/HazardEditor.h"

namespace Editor
{
	using namespace Hazard;

	void EditorWorldManager::Init()
	{
		WorldRendererSpec spec = {};
		spec.DebugName = "MainWorld";
		spec.TargetWorld = Application::Get().GetModule<WorldHandler>().GetCurrentWorld();
		s_WorldRenderer = Ref<WorldRenderer>::Create(&spec);
	}
	void EditorWorldManager::Update()
	{
		HZR_PROFILE_FUNCTION();
		s_WorldRenderer->Submit();
	}
	void EditorWorldManager::LoadWorld(AssetHandle handle)
	{
		Timer timer;
        Promise<Ref<World>> promise = AssetManager::GetAssetAsync<World>(handle);
		promise.ContinueWith([timer](const auto& results) {
            Ref<World> result = results[0];
			if (!result) return;

			SetWorld(result);
			HZR_INFO("World was loaded in {}ms", timer.ElapsedMillis());
		});
	}
	void EditorWorldManager::SetWorld(Ref<Hazard::World> world)
	{
		auto& handler = Application::Get().GetModule<WorldHandler>();
		handler.SetWorld(world);
		s_WorldRenderer->SetTargetWorld(handler.GetCurrentWorld());

		Events::SelectionContextChange e({});
		HazardLoop::GetCurrent().OnEvent(e);
	}
}
