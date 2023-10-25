#pragma once

#include "Hazard.h"
#include "Core/Window.h"
#include "Platform/EditorGuiPlatform.h"
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
    
    Hazard::ImUI::PanelManager& GetPanelManager() { return m_PanelManager; }
	Hazard::ImUI::MenuBar& GetMenuBar() { return m_MainMenuBar; }
	Ref<HazardRenderer::Sampler>& GetImageSampler() { return m_ImageSampler; }

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
	Ref<HazardRenderer::Sampler> m_ImageSampler;
	HazardRenderer::Window* m_Window;
	UI::MainMenuBar m_MainMenuBar;
};
