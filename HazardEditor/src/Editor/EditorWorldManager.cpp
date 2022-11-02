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

		m_WorldRenderer = Ref<WorldRenderer>::Create(&spec);

		Application::GetModule<WorldHandler>().SetWorld(m_WorldRenderer->GetTargetWorld());
	}
	void EditorWorldManager::Update()
	{
		HZR_PROFILE_FUNCTION();
		m_WorldRenderer->Submit();
	}
}