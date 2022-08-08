#pragma once

#include "Hazard.h"
#include "HazardRendererCore.h"
#include "Platform/EditorGuiPlatform.h"

class GUIManager : public Hazard::Module 
{
public:
	GUIManager() : Hazard::Module("GUIManager") {};
	~GUIManager() = default;

	void Init() override;
	void Update() override;
	void Render() override;

	bool OnEvent(Event& e) override;
    
    Hazard::ImUI::PanelManager& GetPanelManager() { return m_PanelManager; }

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
	Hazard::ImUI::PanelManager m_PanelManager;
	EditorGuiPlatform* m_Platform;
	HazardRenderer::Window* m_Window;
};
