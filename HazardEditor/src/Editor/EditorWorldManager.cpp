#include "EditorWorldManager.h"
#include "Hazard/ECS/WorldHandler.h"

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
		auto& handler = Application::GetModule<WorldHandler>();
		auto promise = handler.LoadWorldAsync(path).Then([](JobSystem* system, Job* job) -> size_t {
			auto& handler = Application::GetModule<WorldHandler>();
			s_WorldRenderer->SetTargetWorld(handler.GetCurrentWorld());
			return 0;
			});
	}
}