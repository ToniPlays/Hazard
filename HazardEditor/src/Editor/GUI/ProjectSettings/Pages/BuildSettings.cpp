#include "BuildSettings.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/ImGUI/UILibrary.h"
#include "Hazard/ImGUI/ScopedVar.h"
#include "Project/ProjectManager.h"


BuildSettings::BuildSettings()
{
	m_BuildDirectory.SetHint("Directory to build to");
	m_BuildPathFormat.SetHint("Automatic build directory name");


	m_BuildWorlds = Hazard::ImUI::Table<Hazard::AssetMetadata>("Target worlds", { 0, 0 }, true);
	m_BuildWorlds.SetColumns({ "Source", "Actions" });
	m_BuildWorlds.RowHeight(32.0f);
	m_BuildWorlds.RowContent([](uint32_t index, Hazard::AssetMetadata& metadata) {
		Hazard::ImUI::ShiftX(4.0f);
		ImGui::Text("%s", File::GetNameNoExt(metadata.SourceFile).c_str());
		ImGui::TableNextColumn();
		Hazard::ImUI::ShiftX(4.0f);
		ImGui::Text("Maybe something");
	});
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

	for (auto& metadata : project.GetSettings().BuildSettings.TargetWorlds)
		m_BuildWorlds.AddRow(metadata);

	m_PreviewValue = project.GetBuildTargetName();
}

void BuildSettings::OnClose()
{
	if (!m_ShouldSave) return;
	m_ShouldSave = false;

	HazardProject& project = ProjectManager::GetCurrentProject();
	project.GetSettings().BuildSettings.TargetWorlds = m_BuildWorlds.GetRows();


	project.GetSettings().BuildSettings.Save();

	m_BuildWorlds.ClearRows();
}

void BuildSettings::RenderPage()
{
	using namespace Hazard;
	const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

	RenderSharedSettings();
	ImUI::Shift(4, 16);

	ImVec2 size = ImGui::GetContentRegionAvail();

	m_BuildWorlds.Size({ size.x - 8.0f, 0.0f });
	m_BuildWorlds.Render();

	ImUI::DropTarget<AssetHandle>(AssetType::World, [&](AssetHandle handle) {
		AssetMetadata& metadata = AssetManager::GetMetadata(handle);
		m_BuildWorlds.AddRow(metadata);
		m_ShouldSave = true;
	});
}

void BuildSettings::RenderSharedSettings()
{
	using namespace Hazard;

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, 150);
	ImGui::Text("%s", "Build directory");
	ImGui::NextColumn();

	m_BuildDirectory.Render();
	ImGui::NextColumn();

	ImGui::Text("%s", "Target name");
	ImGui::NextColumn();
	m_BuildPathFormat.Render();

	ImGui::Columns();

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
