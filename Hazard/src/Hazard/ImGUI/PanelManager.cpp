
#include <hzrpch.h>
#include "PanelManager.h"

namespace Hazard::ImUI
{
	PanelManager::PanelManager()
	{
		s_Instance = this;
	}
	void PanelManager::Update()
	{
		HZR_PROFILE_FUNCTION();
		for (GUIRenderable* renderable : m_Renderables)
			renderable->Update();
	}
	void PanelManager::Render()
	{
		HZR_PROFILE_FUNCTION();
		HZR_TIMED_FUNCTION();
		for (GUIRenderable* renderable : m_Renderables) 
			renderable->Render();
	}
}
