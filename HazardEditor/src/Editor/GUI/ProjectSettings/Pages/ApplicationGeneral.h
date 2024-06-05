#pragma once

#include "SettingPage.h"
#include "Hazard/ImGUI/UIElements/TextField.h"

class ApplicationGeneral : public SettingPage
{
public:
	ApplicationGeneral();

	const char* GetPageTitle() const override;
	const char* GetPageDescription() const override;

	void OnOpen() override;
	void OnClose() override;
	void RenderPage() override;

private:
	Hazard::ImUI::TextField m_CompanyName;
	Hazard::ImUI::TextField m_ProjectName;
	Hazard::ImUI::TextField m_ProjectVersion;

	bool m_ShouldSave = false;
};