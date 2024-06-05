#pragma once

#include "ApplicationGeneral.h"
#include <Hazard/ImGUI/UILibrary.h>
#include <Hazard/ImGUI/ScopedVar.h>
#include <Hazard/ImGUI/Style.h>
#include <Hazard/ImGUI/StyleManager.h>

#include "Project/ProjectManager.h"

ApplicationGeneral::ApplicationGeneral()
{

}

const char* ApplicationGeneral::GetPageTitle() const
{
	return "General - Application";
}

const char* ApplicationGeneral::GetPageDescription() const
{
	return "This is for application settings";
}

void ApplicationGeneral::OnOpen()
{
	HazardProject& project = ProjectManager::GetCurrentProject();

	m_CompanyName.SetHint("Company name");
	m_CompanyName.SetValue(project.GetSettings().RuntimeConfig.CompanyName);

	m_ProjectName.SetHint("Project name");
	m_ProjectName.SetValue(project.GetSettings().RuntimeConfig.ProjectName);

	m_ProjectVersion.SetHint("Version identifier");
	m_ProjectVersion.SetValue(project.GetSettings().RuntimeConfig.VersionIdentifier);
}

void ApplicationGeneral::OnClose()
{
	if (!m_ShouldSave) return;
	m_ShouldSave = false;

	HazardProject& project = ProjectManager::GetCurrentProject();
	project.GetSettings().RuntimeConfig.Save();
}

void ApplicationGeneral::RenderPage()
{
	using namespace Hazard;
	ImVec2 size = ImGui::GetContentRegionAvail();
	
	ImUI::Style& style = ImUI::StyleManager::GetCurrent();

	ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.BackgroundColor);
	ImUI::ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, style.Window.Rounding);

	ImGui::BeginChild("##prop", { size.x - 8, 108 }, false);

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, 150);

	ImUI::Shift(4, 8);
	ImGui::Text("Company name");
	ImGui::NextColumn();
	ImUI::ShiftY(4);
	m_CompanyName.Render();
	ImGui::NextColumn();

	ImUI::Shift(4, 8);
	ImGui::Text("Project name");
	ImGui::NextColumn();
	ImUI::ShiftY(4);
	m_ProjectName.Render();
	ImGui::NextColumn();

	ImUI::Shift(4, 8);
	ImGui::Text("Project version");
	ImGui::NextColumn();
	ImUI::ShiftY(4);
	m_ProjectVersion.Render();

	ImGui::Columns();

	if (m_CompanyName.DidChange() || m_ProjectName.DidChange() || m_ProjectVersion.DidChange())
	{
		HazardProject& project = ProjectManager::GetCurrentProject();
		project.GetSettings().RuntimeConfig.CompanyName = m_CompanyName.GetValue();
		project.GetSettings().RuntimeConfig.ProjectName = m_ProjectName.GetValue();
		project.GetSettings().RuntimeConfig.VersionIdentifier = m_ProjectVersion.GetValue();

		m_ShouldSave = true;
	}

	ImGui::EndChild();
}
