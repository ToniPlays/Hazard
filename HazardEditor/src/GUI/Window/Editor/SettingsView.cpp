#pragma once
#include <hzreditor.h>
#include "SettingsView.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Window/DragDropUtils.h"
#include "Project/ProjectManager.h"

using namespace WindowLayout;
namespace WindowElement
{
	SettingsView::SettingsView() : EditorWindow("Settings", ImGuiWindowFlags_NoDocking)
	{
		SetActive(false);
	}
	SettingsView::~SettingsView()
	{

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
		Project::ProjectManager& manager = Application::GetModule<Project::ProjectManager>();
		ImGui::Text("Project settings");
		using namespace Hazard::Rendering;
		Ref<Texture2D> texture = Ref(Vault::Get<Texture2D>("res/icons/world.png"));

		Layout::Table(2, false);
		Layout::SetColumnWidth(150);
		Layout::Text("Editor startup world");
		Layout::TableNext();
		Layout::MaxWidth();

		if (Input::ImageButton(texture->GetID())) {
		
		}
		DragDropUtils::DragTarget("World", [&](const ImGuiPayload* payload) {
			const char* file = (const char*)payload->Data;
			manager.Set("Editor.StartupWorld", file);
			});
		ImGui::SameLine(0, 5);
		ImGui::Text(File::GetNameNoExt(manager.GetProject().m_StartupWorld).c_str());

		Layout::TableNext();
		Layout::EndTable();
	}
}
