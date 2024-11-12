#pragma once

#include "Hazard/ImGUI/Panel.h"
#include "Hazard/ImGUI/UIElements/TextField.h"
#include "Core/HazardLauncherManager.h"

//TODO: FIX
#define CONFIG_PATH "LauncherConfig.hzrcfg"

class LauncherGUI : public Hazard::ImUI::GUIRenderable
{
public:
	LauncherGUI();
	~LauncherGUI() = default;

	bool OnEvent(Event& e) override { return false; }

	void Render() override;
	void DrawSidebar();
	void DrawBottomBar();

private:
	Hazard::ImUI::TextField m_SearchField;
	Hazard::ImUI::TextField m_NewProjectPath;
	Hazard::ImUI::TextField m_NewProjectName;

	HazardLauncherManager m_Manager;

	bool m_EnvVarExists = false;
};
