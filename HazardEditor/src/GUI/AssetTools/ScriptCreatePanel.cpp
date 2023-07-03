
#include "ScriptCreatePanel.h"
#include "Hazard/ImGUI/UILibrary.h"
#include "Editor/EditorScriptManager.h"
#include "Core/GUIManager.h"
#include "GUI/ProjectPanel/AssetPanel.h"
#include "Core/HazardEditor.h"
#include "Localization/Localization.h"

using namespace Hazard;

namespace UI
{
	ScriptCreatePanel::ScriptCreatePanel() : Hazard::ImUI::Modal(LBL_SCRIPT_CREATE_PANEL, { 600, 700 })
	{
		m_CreateInfo.SetDefaults();
	}
	bool ScriptCreatePanel::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(ScriptCreatePanel::OnKeyPressed));
	}
	bool ScriptCreatePanel::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::Escape) {
			Close();
			return true;
		}
		return false;
	}
	void ScriptCreatePanel::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 150);
		ImUI::Group("Name", [&]() {
			ImGui::Text("Class name");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			ImUI::TextFieldWithHint(m_CreateInfo.ClassName, "Class name");
			ImGui::NextColumn();
			});
		ImUI::Group("Derive", [&]() {
			ImGui::Text("Derives from");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			ImUI::TextFieldWithHint(m_CreateInfo.Derives, "Derives from");
			});

		ImUI::ShiftY(16.0f);
		ImGui::NextColumn();

		DrawMethodList();

		ImGui::NextColumn();
		ImGui::Text("File path");
		ImGui::NextColumn();
		std::string path = m_Directory.string() + "/" + m_CreateInfo.ClassName + ".cs";
		ImGui::Text("%s", path.c_str());
		ImGui::Columns();


		ImVec2 buttonPos = ImGui::GetWindowSize();
		buttonPos.x -= 104.0f;
		buttonPos.y -= 28.0f;
		ImGui::SetCursorPos(buttonPos);

		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImUI::ScopedColourStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);

		ImGui::BeginDisabled(m_CreateInfo.ClassName.empty());
		if (ImGui::Button("Create", { 100, 24 })) {
			m_CreateInfo.Path = path;
			/*
			if (EditorAssetManager::CreateScriptAsset(m_CreateInfo))
			{
				m_CreateInfo.SetDefaults();
				auto& manager = Application::GetModule<Editor::EditorScriptManager>();
				manager.RecompileAndLoad();

				auto* assetPanel = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<AssetPanel>();
				assetPanel->RefreshFolderItems();
				((HazardEditorApplication&)Application::Get()).GetScriptManager().RecompileAndLoad();
				Close();
			}
			*/
		}
		ImGui::EndDisabled();
	}
	void ScriptCreatePanel::DrawMethodList()
	{
		ImUI::ScopedStyleVar rounding(ImGuiStyleVar_FrameRounding, 4);
		ImGui::Text("Include OnCreate()");
		ImGui::NextColumn();

		static bool onCreate = false;

		if (ImGui::Checkbox("OnCreate", &onCreate)) 
		{
			if (onCreate)
				m_CreateInfo.Methods[MethodImpl::OnCreate] = "\tpublic void OnCreate()\n\t{\n\t\n\t}";
			else m_CreateInfo.Methods.erase(MethodImpl::OnCreate);
		}
		ImGui::NextColumn();

		ImGui::Text("Include OnUpdate");
		ImGui::NextColumn();

		static bool onUpdate = false;
		if (ImGui::Checkbox("OnUpdate(float deltaTime)", &onUpdate)) {
			if (onUpdate)
				m_CreateInfo.Methods[MethodImpl::OnUpdate] = "\tpublic void OnUpdate(float deltaTime)\n\t{\n\t\n\t}";
			else m_CreateInfo.Methods.erase(MethodImpl::OnUpdate);
		}

		ImGui::NextColumn();
		ImGui::Text("Include OnLateUpdate");
		ImGui::NextColumn();

		static bool onLateUpdate = false;
		if (ImGui::Checkbox("OnLateUpdate()", &onLateUpdate)) {
			if (onLateUpdate)
				m_CreateInfo.Methods[MethodImpl::OnLateUpdate] = "\tpublic void OnLateUpdate(float timeStep)\n\t{\n\t\n\t}";
			else m_CreateInfo.Methods.erase(MethodImpl::OnLateUpdate);
		}
		ImGui::NextColumn();
		ImGui::Text("Include OnDestroy");
		ImGui::NextColumn();

		static bool onDestroy = false;
		if (ImGui::Checkbox("OnDestroy()", &onDestroy)) {
			if (onDestroy)
				m_CreateInfo.Methods[MethodImpl::OnDestroy] = "\tpublic void OnDestroy()\n\t{\n\t\n\t}";
			else m_CreateInfo.Methods.erase(MethodImpl::OnDestroy);
		}
	}
}
