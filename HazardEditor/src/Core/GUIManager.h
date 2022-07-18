#pragma once

#include "Hazard.h"
#include "GUIRenderable.h"
#include "HazardRendererCore.h"
#include "Platform/EditorGuiPlatform.h"
#include "GUI/Core/PanelManager.h"
#include "GUI/MainMenuBar.h"

class GUIManager : public Hazard::Module 
{
public:
	GUIManager() : Hazard::Module("GUIManager") {};
	~GUIManager() = default;

	void Init() override;
	void Update() override;
	void Render() override;

	bool OnEvent(Event& e) override;
    
    UI::PanelManager& GetPanelManager() { return m_PanelManager; }
    UI::MenuBar& GetMenuBar() { return m_MainMenuBar; }

	template<typename T>
	void SetPanelOpen(bool open) 
	{
		T* renderable = m_PanelManager.GetRenderable<T>();
		if(open)
			renderable->Open();
		else renderable->Close();
	}
    
private:
	void InitImGuiPlatform(HazardRenderer::Window& window);

private:
	UI::PanelManager m_PanelManager;
	EditorGuiPlatform* m_Platform;
	HazardRenderer::Window* m_Window;
	UI::MainMenuBar m_MainMenuBar;
};
