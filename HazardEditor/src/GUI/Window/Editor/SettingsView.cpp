#pragma once

#include "SettingsView.h"
#include "Library/Input.h"
#include "Library/Style.h"
#include "Library/Layout/Layout.h"
#include "Library/DragDropUtils.h"
#include "Project/ProjectManager.h"
#include "Hazard/Assets/AssetManager.h"

using namespace WindowLayout;
namespace WindowElement
{
	SettingsView::SettingsView() : EditorWindow("Settings", ImGuiWindowFlags_NoDocking)
	{
		SetActive(false);
		
		m_Images.clear();
		AssetHandle handle = Hazard::AssetManager::GetHandleFromFile("icons/world.png");
		m_Images["world"] = Hazard::AssetManager::GetAsset<Hazard::Rendering::Texture2D>(handle);

	}
	void SettingsView::OnWindowRender()
	{
		Layout::Table(2, true);
		float height = ImGui::GetContentRegionAvail().y - 39;
		ImGui::BeginChild("##menus", {0, height });
		CreateMenus();
		ImGui::EndChild();
		Layout::TableNext();

		ImGui::BeginChild("##content", { 0, height });
		Layout::MaxWidth();
		Input::InputField(m_SearchValue, "Search");
		Layout::Separator(3);
		CreateContent();
		ImGui::EndChild();
		Layout::EndTable();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#0D0D0B")));
		ImGui::BeginChild("##settingsBar", {0, 35});

		ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() - 85);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

		if (Input::Button("Save", { 80, 25 })) {
			Application::GetModule<Project::ProjectManager>().Save();
		}
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
	void SettingsView::OnBeforeRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	}
	void SettingsView::OnAfterRender()
	{
		ImGui::PopStyleVar();
	}
	void SettingsView::CreateMenus()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
		Layout::Treenode("Project settings", Style::GetTreeNodeDefaultFlags(), [&]() {
			if (Input::Button("Project")) {

			}
			});
		Layout::Treenode("Engine settings", Style::GetTreeNodeDefaultFlags(), [&]() {
			if (Input::Button("Rendering")) {

			}
			});
		ImGui::PopStyleVar();
	}
	void SettingsView::CreateContent()
	{
		using namespace Hazard::Rendering;
		Project::ProjectManager& manager = Application::GetModule<Project::ProjectManager>();
		ImGui::Text("Project settings");

		Layout::Table(2, false);
		Layout::SetColumnWidth(150);
		Layout::Text("Editor startup world");
		Layout::TableNext();
		Layout::MaxWidth();

		if (Input::ImageButton(m_Images["world"])) {
			
		}

		DragDropUtils::DragTarget("World", [&](const ImGuiPayload* payload) {
			AssetHandle handle = *(AssetHandle*)payload->Data;
			manager.Set("Editor.StartupWorld", AssetManager::GetMetadata(handle).Path.string());
			}); 

		ImGui::SameLine(0, 5);
		ImGui::Text(File::GetNameNoExt(manager.GetProject().StartupWorld).c_str());

		Layout::TableNext();
		Layout::EndTable();
	}
}
