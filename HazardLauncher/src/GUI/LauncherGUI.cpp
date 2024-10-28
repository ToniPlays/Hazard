#include "LauncherGUI.h"
#include "Hazard.h"
#include "Core/HazardProject.h"

#include "Hazard/ImGUI/UIElements/Table.h"
#include "Hazard/ImGUI/UILibrary.h"
#include <Directory.h>
#include <Platform/OS.h>

LauncherGUI::LauncherGUI() 
{
	m_EnvVarExists = OS::GetEnv("HAZARD_DIR");
	m_Manager.LoadFromConfigFile(CONFIG_PATH);
}

void LauncherGUI::Render()
{
	using namespace Hazard;

	const ImUI::Style& style = ImUI::StyleManager().GetCurrent();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("HazardLauncher", 0, window_flags);

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, 250);
	DrawSidebar();

	ImGui::NextColumn();
	ImVec2 contentSize = ImGui::GetContentRegionAvail();
	contentSize.y -= 125.0f;

	ImGui::BeginChild("Content", contentSize);

	m_SearchField.Render();

	ImUI::Table<HazardProject> table("ProjectTable", ImGui::GetContentRegionAvail());
	table.SetColumns({ "Project", "Version", "Modified" });
	table.RowHeight(36.0f);
	table.RowContent([style](uint32_t, const HazardProject& project) {
		ImUI::Separator({ 2.0f, 36.0f }, style.Colors.AxisZ);
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::SameLine(0, 8.0f);
		ImGui::Text("%s", project.Name.c_str());
		ImGui::PopFont();
		ImGui::TableNextColumn();

		ImUI::ShiftX(8.0f);
		ImGui::Text("0.0.0");

		ImGui::TableNextColumn();
		ImUI::ShiftX(8.0f);
		ImGui::Text("12.2.2022");
		});

	for (const HazardProject& project : m_Manager.GetProjects())
		table.AddRow(project);

	table.Render();

	if (table.DidSelect())
	{
		if (m_Manager.OpenProject(table.SelectedValue()))
		{
			WindowCloseEvent e;
			HazardLoop::GetCurrent().OnEvent(e);
		}
		else
			ImGui::OpenPopup("ErrorPopup");
	}

	ImGui::EndChild();
	ImGui::Columns();
	DrawBottomBar();

	ImGui::End();

	ImGui::PopStyleVar(3);

	if (ImGui::BeginPopup("ErrorPopup"))
	{
		ImGui::EndPopup();
	}

	if (!m_EnvVarExists)
	{
		ImGui::OpenPopup("Install location");
	}

	if (ImGui::BeginPopupModal("Install location"))
	{
		if (ImGui::Button("Set installation folder"))
		{
			std::filesystem::path hazardDir = Directory::OpenFolderDialog();
			if (!hazardDir.empty())
				if (OS::SetEnv("HAZARD_DIR", hazardDir.string().c_str()))
				{
					m_EnvVarExists = true;
					ImGui::CloseCurrentPopup();
				}
		}
		ImGui::EndPopup();
	}
}
void LauncherGUI::DrawSidebar()
{
	ImVec2 size = ImGui::GetContentRegionAvail();
	size.y -= 125.0f;
	ImGui::BeginChild("Sidebar", size);

	ImGui::EndChild();
}
void LauncherGUI::DrawBottomBar()
{
	using namespace Hazard;
	const ImUI::Style& style = ImUI::StyleManager().GetCurrent();
	ImVec2 size = ImGui::GetContentRegionAvail();

	ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.BackgroundColor);

	ImGui::BeginChild("BottomNavBar", size);
	ImUI::Separator({ size.x, 2.0f }, style.Window.HeaderActive);
	ImUI::Shift(16.0, 32.0f);

	ImUI::ShiftY(4.0f);
	ImGui::Text("Project Location");
	ImGui::SameLine(0, 5.0f);
	ImUI::ShiftY(-4.0f);
	ImGui::BeginChild("##path", { 350, 28 });

	m_NewProjectPath.Render();

	ImGui::EndChild();
	{
		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImUI::ScopedColorStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);
		ImGui::SameLine(0, 5.0f);
		if (ImGui::Button("Browse", { 100, 28 }))
		{
			std::filesystem::path newPath = Directory::OpenFolderDialog();
			if (!newPath.empty())
				m_NewProjectPath.SetValue(newPath.string());
		}

		ImGui::SameLine(0, 356.0f);
		ImGui::Text("Project name");
		ImGui::SameLine(0, 5.0f);

		ImGui::SetNextItemWidth(250.0f);
		ImGui::PushID("##name");

		m_NewProjectName.Render();
		ImGui::PopID();

		ImGui::SetCursorPos({ size.x - 216.0f, size.y - 32.0f });
		if (ImGui::Button("Create", { 100, 28 }))
		{
			if (!m_NewProjectPath.GetValue().empty())
			{
				HazardProject project = {};
				project.Name = m_NewProjectName.GetValue();
				project.Path = std::filesystem::path(m_NewProjectPath.GetValue()) / project.Name;

				if (m_Manager.CreateProject(project))
					m_Manager.SaveConfigToFile(CONFIG_PATH);

			}
			else HZR_ERROR("Path cannot be empty");
		}
		ImGui::SameLine(0, 8);

		if (ImGui::Button("Add project", { 100, 28 }))
		{
			std::filesystem::path newPath = File::OpenFileDialog({ "All files", "* ", "Hazard Project (.hzrproj)", "*.hzrproj" });
			if (!newPath.empty())
			{
				if (m_Manager.ImportProject(newPath))
					m_Manager.SaveConfigToFile(CONFIG_PATH);
				else
				{
					ImGui::OpenPopup("ErrorPopup");
				}
			}
		}
	}
	ImGui::EndChild();
}