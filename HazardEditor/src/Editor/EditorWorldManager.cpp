#include "EditorWorldManager.h"

namespace Editor 
{
	using namespace HazardRenderer;

	void EditorWorldManager::Init()
	{
		WorldRendererSpec spec = {};
		spec.Geometry = Geometry_All;
		spec.DebugName = "MainWorld";

		m_WorldRenderer = Ref<WorldRenderer>::Create(&spec);
	}
	void EditorWorldManager::Update()
	{
		if (!m_WorldRenderer->IsValid()) return;
		m_WorldRenderer->Submit();
	}
}