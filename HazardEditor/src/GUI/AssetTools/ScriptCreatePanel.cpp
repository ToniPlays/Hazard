
#include "ScriptCreatePanel.h"
#include "Hazard/ImGUI/UILibrary.h"
#include "Editor/EditorScriptManager.h"
#include "Core/GUIManager.h"
#include "GUI/ProjectPanel/AssetPanel.h"

using namespace Hazard;
namespace UI
{
	ScriptCreatePanel::ScriptCreatePanel() : Hazard::ImUI::Modal("Create script", { 600, 700 }) 
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
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 150);
		ImUI::Group("Name", [&]() {
			ImGui::Text("Class name");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
			ImUI::TextField(m_CreateInfo.ClassName, "Class name");
			ImGui::NextColumn();
			});
		ImUI::Group("Derive", [&]() {
			ImGui::Text("Derives from");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
			ImUI::TextField(m_CreateInfo.Derives, "Derives from");
			});

		ImGui::NextColumn();
		ImGui::Text("File path");
		ImGui::NextColumn();
		std::string path = m_Directory.string() + "/" + m_CreateInfo.ClassName + ".cs";
		ImGui::Text(path.c_str());
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

			m_CreateInfo.Methods.push_back("\tpublic void OnCreate() {\n\t\n\t}");
			m_CreateInfo.Methods.push_back("\tpublic void OnUpdate(float timestep) {\n\t\n\t}");

			if (EditorAssetManager::CreateScriptAsset(m_CreateInfo)) 
			{
				m_CreateInfo.SetDefaults();
				auto& manager = Application::GetModule<Editor::EditorScriptManager>();
				manager.RecompileAndLoad();
				
				auto* assetPanel = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<AssetPanel>();
				assetPanel->RefreshFolderItems();
				Close();
			}
		}
		ImGui::EndDisabled();
	}
}