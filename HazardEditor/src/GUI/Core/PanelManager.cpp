
#include "PanelManager.h"
#include "GUI/MainMenuBar.h"
#include "GUI/AllPanels.h"

namespace UI 
{
	PanelManager::PanelManager()
	{
		s_Instance = this;
		AddRenderable<RenderCommandListPanel>();
		AddRenderable<RendererDebugPanel>();
		AddRenderable<Viewport>();
		AddRenderable<Properties>();
		AddRenderable<Hierarchy>();
		AddRenderable<AssetPanel>();
		AddRenderable<Console>();
	}
	void PanelManager::Update()
	{
		HZR_PROFILE_FUNCTION();
		for (GUIRenderable* renderable : m_Renderables)
		{
			renderable->Update();
		}
	}
	void PanelManager::Render()
	{
		HZR_PROFILE_FUNCTION();
		for (GUIRenderable* renderable : m_Renderables) 
		{
			renderable->Render();
		}
	}
}
