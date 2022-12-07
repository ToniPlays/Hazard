#include "EditorWorldManager.h"
#include "Hazard/ECS/WorldHandler.h"
#include "Core/GUIManager.h"
#include "GUI/Overlays/ProgressOverlay.h"

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
	void EditorWorldManager::LoadWorld(const std::filesystem::path& path)
	{
		WorldAsyncPromises promises;
		auto& handler = Application::GetModule<WorldHandler>();
		handler.LoadWorld(path, Serialization::Editor);
		s_WorldRenderer->SetTargetWorld(handler.GetCurrentWorld());

		/*
		auto& handler = Application::GetModule<WorldHandler>();
		auto promise = handler.LoadWorldAsync(path, Serialization::Editor, &promises);

		auto thenPromise = promise.Then([](JobSystem* system, Job* job) -> size_t {
			auto& handler = Application::GetModule<WorldHandler>();
			s_WorldRenderer->SetTargetWorld(handler.GetCurrentWorld());
			return 0;
			});

		auto panel = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<UI::ProgressOverlay>();
		panel->AddProcess(AssetType::World, promises.WorldPromise);

		for (auto& promise : promises.AssetPromises)
			panel->AddProcess(promise.Type, promise.Promise);
		*/
	}
}