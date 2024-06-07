#pragma once

#include "SettingPage.h"
#include "Project/ProjectSettingsSchema.h"
#include "Hazard/ImGUI/UIElements/TextField.h"
#include "Hazard/ImGUI/UIElements/Table.h"

#include "Hazard/Assets/Asset.h"

class BuildSettings : public SettingPage
{
public:
	BuildSettings();
	~BuildSettings() = default;

	const char* GetPageTitle() const override;
	const char* GetPageDescription() const override;

	void OnOpen() override;
	void OnClose() override;
	void RenderPage() override;

private:
	void RenderSharedSettings();
	void RenderPlatformTab(Platform platform);

private:
	Hazard::ImUI::TextField m_BuildDirectory;
	Hazard::ImUI::TextField m_BuildPathFormat;

	Hazard::ImUI::Table<Hazard::AssetMetadata> m_BuildWorlds;

	std::string m_PreviewValue;
	bool m_ShouldSave = false;
};