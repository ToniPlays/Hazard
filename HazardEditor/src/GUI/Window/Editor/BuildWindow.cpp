#pragma once

#include "BuildWindow.h"
#include "library/Layout/Layout.h"
#include "imgui.h"
#include "Library/Input.h"
#include "Library/DragDropUtils.h"
#include "Project/ProjectManager.h"


using namespace WindowLayout;

namespace WindowElement
{
	BuildWindow::BuildWindow() : EditorWindow(ICON_FK_COGS" Build settings", ImGuiWindowFlags_NoDocking)
	{
		SetActive(false);
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

		exporter.SetBuildSettings(m_BuildSettings);
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
		//Underlined bar
		{
			Layout::Table(4, false, "Build stats");

			ImGui::Text(m_Report.Result ? ICON_FK_CHECK_SQUARE " Build succeeded" : ICON_FK_TIMES" Build failed");
			Layout::TableNext();
			std::stringstream ss;

			ss << "Size: " << Hazard::StringUtil::BytesToString(m_Report.TotalSize);

			ImGui::Text(ss.str().c_str());
			Layout::TableNext();
			ImGui::Text("Time: %.2f", m_Report.BuildTime / 1000.0f);

			Layout::TableNext();
			ImGui::Text("Something random here");

			Layout::EndTable();
		}
		Layout::Table(2, false, "#restypes");
		//Assets		
		{
			Layout::ShiftY(25.0f);
			for (auto [type, size] : m_Report.AssetTypeSize) {
				float val = (float)size / (float)m_Report.TotalSize;
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, Style::AssetTypeColor(type));
				ImGui::ProgressBar((float)size / (float)m_Report.TotalSize, ImVec2(300, 25));
				Layout::SameLine();
				Layout::ShiftX(15);
				ImGui::PopStyleColor();
				ImGui::Text(Hazard::Utils::AssetTypeToString(type));
				Layout::ShiftY(5.0f);
			}
		}
		ImGui::NextColumn();
		//Assets		
		{
			ScopedColour color(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#0D0D0B")));
			ImGui::BeginChild("resource", ImVec2(0, -50.0f), false);

			for (auto [type, resources] : m_Report.Resources)
			{
				for (auto& resources : resources) {
					Layout::ShiftX(5.0f);
					ImGui::Text(resources.FileName.c_str());
				}
			}
			ImGui::EndChild();
		}
		Layout::EndTable();
		//Bottom panel
		{
			ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMax().y - 35);
			ScopedColour childBG(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#0D0D0B")));
			ImGui::BeginChild("##settingsBar", { 0, 35 });


			ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() - 85);
			Layout::ShiftY(5.0f);
			if (Input::Button("Save", { 80, 25 }))
			{
				SetActive(false);
				m_HasBuilt = false;
			}
			ImGui::EndChild();
		}
	}
	void BuildWindow::ShowBuildSettings()
	{
		//Title bar
		{
			Style::SelectFont(2);
			Layout::ShiftY(5.0f);
			float textWidth = ImGui::CalcTextSize("Build settings").x;
			ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - textWidth) / 2.0f);
			ImGui::Text("Build settings");
			ImGui::PopFont();

			Layout::Underline(true, 0.0f, 5.0f);
			Layout::ShiftY(10.0f);
		}

		{
			ImGui::Text("Included worlds");
			ImVec2 avail = ImGui::GetContentRegionAvail();
			ScopedColour childBG(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#0D0D0B")));
			Layout::Shift(10, 5);
			ImGui::BeginChild("##buildWorlds", { avail.x - 20, avail.y - 55 });
			
			ImVec2 offset = ImGui::GetCursorPos();
			ImVec2 windowSize = ImVec2(offset.x + avail.x, offset.y + avail.y);

			DragDropUtils::DragTargetCustom("World", ImRect(offset, windowSize), ImGuiID("worldDrag"), [&](const ImGuiPayload* payload) {
				AssetHandle handle = *(AssetHandle*)payload->Data;
				m_BuildSettings.m_Worlds.push_back(AssetManager::GetMetadata(handle));
				});

			for (auto world : m_BuildSettings.m_Worlds)
			{
				ImGui::Text(world.Path.string().c_str());
			}

			ImGui::EndChild();

		}
		//Bottom panel
		{
			ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMax().y - 35);
			ScopedColour childBG(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#0D0D0B")));
			ImGui::BeginChild("##settingsBar", { 0, 35 });

			ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() - 85 - 100);
			Layout::ShiftY(5.0f);

			if (Input::Button("Save", { 80, 25 }))
			{
				std::string& projectPath = Application::GetModule<Project::ProjectManager>().GetProject().AbsolutePath;
				Exporter::GameExportManager manager;
				manager.SaveBuildSettings(projectPath + "/editor/buildsettings.hzrbld", m_BuildSettings);
			}
			ImGui::SameLine();

			if (Input::Button("Build", { 80, 25 }))
			{
				std::filesystem::path path = File::SaveFolderDialog();
				if (path.string() != "")
					ExportProject(path);
			}
			ImGui::EndChild();
		}
	}
}
