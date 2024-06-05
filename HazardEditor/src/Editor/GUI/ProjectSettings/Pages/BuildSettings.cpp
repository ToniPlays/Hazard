#include "BuildSettings.h"
#include "Hazard/ImGUI/UILibrary.h"
#include "Hazard/ImGUI/ScopedVar.h"
#include "Project/ProjectManager.h"


BuildSettings::BuildSettings()
{
	m_BuildDirectory.SetHint("Directory to build to");
	m_BuildPathFormat.SetHint("Automatic build directory name");
}

const char* BuildSettings::GetPageTitle() const
{
	return "Runtime - Build settings";
}

const char* BuildSettings::GetPageDescription() const
{
	return "This is for build settings";
}

void BuildSettings::OnOpen()
{
	HazardProject& project = ProjectManager::GetCurrentProject();
	m_BuildDirectory.SetValue(project.GetSettings().BuildSettings.BuildDirectory.string());
	m_BuildPathFormat.SetValue(project.GetSettings().BuildSettings.BuildTargetFormat);

	m_PreviewValue = project.GetBuildTargetName();
}

void BuildSettings::OnClose()
{
	if (!m_ShouldSave) return;
	m_ShouldSave = false;

	HazardProject& project = ProjectManager::GetCurrentProject();
	project.GetSettings().BuildSettings.Save();
}

void BuildSettings::RenderPage()
{
	using namespace Hazard::ImUI;

	RenderSharedSettings();

	const Style& style = StyleManager::GetCurrent();
	ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(16, 4));
	ScopedStyleColor bgColor(ImGuiCol_ChildBg, style.BackgroundColor);
	ImVec2 size = ImGui::GetContentRegionAvail();

	if (!ImGui::BeginTabBar("##settings"))
		return;

	for (uint32_t i = 0; i < (uint32_t)Platform::PlayStation; i++)
		RenderPlatformTab((Platform)i);

	ImGui::EndTabBar();
}

void BuildSettings::RenderSharedSettings()
{
	using namespace Hazard;

	m_BuildDirectory.Render();
	ImUI::ShiftX(8.0f);
	m_BuildPathFormat.Render();

	if (m_BuildDirectory.DidChange() || m_BuildPathFormat.DidChange())
	{
		HazardProject& project = ProjectManager::GetCurrentProject();
		project.GetSettings().BuildSettings.BuildDirectory = m_BuildDirectory.GetValue();
		project.GetSettings().BuildSettings.BuildTargetFormat = m_BuildPathFormat.GetValue();
		m_PreviewValue = project.GetBuildTargetName();

		m_ShouldSave = true;
	}
	ImUI::Shift(8, 4);
	ImGui::Text("%s", m_PreviewValue.c_str());
	ImUI::Shift(8, 16);
}

void BuildSettings::RenderPlatformTab(Platform platform)
{
	if (!ImGui::BeginTabItem(PlatformToString(platform)))
		return;




	ImGui::EndTabItem();
}
