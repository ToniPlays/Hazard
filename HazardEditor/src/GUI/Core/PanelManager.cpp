
#include "PanelManager.h"
#include "GUI/MainMenuBar.h"
#include "GUI/AllPanels.h"

namespace UI 
{
	PanelManager::PanelManager()
	{
		s_Instance = this;
		AddRenderable<MainMenuBar>();
		AddRenderable<RenderCommandListPanel>();
	}
	void PanelManager::Render()
	{
		for (GUIRenderable* renderable : m_Renderables) {
			renderable->Render();
		}
	}
}
