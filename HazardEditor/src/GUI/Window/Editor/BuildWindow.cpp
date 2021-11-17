#pragma once

#include "BuildWindow.h"
#include "library/Layout/Layout.h"
#include "imgui.h"
#include "Library/Input.h"

using namespace WindowLayout;

namespace WindowElement
{
	BuildWindow::BuildWindow() : EditorWindow(ICON_FK_COGS" Build settings", ImGuiWindowFlags_NoDocking)
	{
		SetActive(true);
	}
	void BuildWindow::OnWindowRender()
	{
		if (m_HasBuilt)
			ShowBuildReport();
		else
			ShowBuildSettings();
	}
	void BuildWindow::ExportProject(const std::filesystem::path& path)
	{
		HZR_CORE_INFO("Exporting game to {0}", path.string());
		using namespace Exporter;
		GameExportManager exporter;

		exporter.SetBuildSettings(BuildSettings());
		exporter.ValidateBuildSettings();
		exporter.ExportGame(path);

		m_Report = exporter.GetBuildReport();
		m_HasBuilt = true;
	}
	void BuildWindow::ShowBuildReport()
	{
		Style::SelectFont(2);
		Layout::ShiftY(5.0f);
		float textWidth = ImGui::CalcTextSize("Build report").x;
		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - textWidth) / 2.0f);
		ImGui::Text("Build report");
		ImGui::PopFont();

		Layout::Underline(true, 0.0f, 5.0f);
		Layout::ShiftY(10.0f);
		Layout::Table(4, false, "Build stats");

		ImGui::Text(m_Report.Result ? "Build succeeded" : "Build failed");
		Layout::TableNext();
		std::stringstream ss;

		ss << "Size: " << Hazard::StringUtil::BytesToString(m_Report.TotalSize);

		ImGui::Text(ss.str().c_str());
		Layout::TableNext();
		ImGui::Text("Time: %.2f", m_Report.BuildTime / 1000.0f);

		Layout::TableNext();
		ImGui::Text("Something random here");

		Layout::EndTable();

		{
			ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMax().y - 35);
			ScopedColour childBG(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#0D0D0B")));
			ImGui::BeginChild("##settingsBar", { 0, 35 });


			ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() - 85);
			Layout::ShiftY(5.0f);
			if (Input::Button("Save", { 80, 25 }))
			{

			}
			ImGui::EndChild();
		}
	}
	void BuildWindow::ShowBuildSettings()
	{
		if (ImGui::Button("Build")) {
			std::filesystem::path path = File::SaveFolderDialog();
			if (path.string() != "")
				ExportProject(path);
		}
	}
}
