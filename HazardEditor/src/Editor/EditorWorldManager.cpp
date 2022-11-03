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
		handler.LoadWorld(path);
		s_WorldRenderer->SetTargetWorld(handler.GetCurrentWorld());
	}
}