#include "PanelManager.h"

#include "GUI/MainMenuBar.h"

namespace UI 
{
	PanelManager::PanelManager()
	{
		s_Instance = this;

		AddRenderable<MainMenuBar>();


	}
	void PanelManager::Render()
	{
		for (GUIRenderable* renderable : m_Renderables) {
			renderable->Render();
		}
	}
}