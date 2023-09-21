#pragma once

#include "Hazard.h"
#include "HazardRendererCore.h"
#include "Platform/EditorGuiPlatform.h"
#include "HazardLauncherManager.h"

#include "Hazard/ImGUI/UIElements/TextField.h"

#define CONFIG_PATH "launcherConfig.hzrcfg"

class GUIManager : public Hazard::Module 
{
public:
	GUIManager() : Hazard::Module("GUIManager") {};
	~GUIManager() = default;

	void Init() override;
	void Update() override;
	void Render() override;

	bool OnEvent(Event& e) override;

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

	void DrawSideBar();
	void DrawBottomBar();

private:
	EditorGuiPlatform* m_Platform;
	HazardRenderer::Window* m_Window;

	Hazard::ImUI::PanelManager m_PanelManager;
	HazardLauncherManager m_Manager;

	Hazard::ImUI::TextField m_SearchField;
	Hazard::ImUI::TextField m_NewProjectPath;

	bool m_EnvVarExists = false;
};
