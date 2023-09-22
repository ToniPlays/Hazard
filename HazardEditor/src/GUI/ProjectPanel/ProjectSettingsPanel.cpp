
#include "ProjectSettingsPanel.h"
#include "Hazard/ImGUI/UIElements/Treenode.h"
#include "Hazard/ImGUI/ScopedVar.h"

namespace UI
{
	using namespace Hazard;

	ProjectSettingsPanel::ProjectSettingsPanel() : Hazard::ImUI::Panel("Project Settings", true)
	{
		m_SearchField = ImUI::TextField("");
		m_SearchField.SetIcon((const char*)ICON_FK_SEARCH);
		m_SearchField.SetHint("Search settings...");
	}
	void ProjectSettingsPanel::OnPanelRender()
	{
		RenderNavbar();
		ImGui::SameLine();

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		ImVec2 size = ImGui::GetContentRegionAvail();
		ImGui::BeginChild("##projectsettings", { size.x, size.y });

		m_SearchField.Render();

		ImUI::Treenode general("General", true);
		general.Content([=]() { DrawInputSettingsPanel(); });
		general.Render();

		ImUI::Treenode inputAxis("Input axis", true);
		inputAxis.Content([=]() { DrawInputSettingsPanel(); });
		inputAxis.Render();

		ImUI::Treenode preprocessor("Script preprocessors", true);
		preprocessor.Content([=]() { DrawInputSettingsPanel(); });
		preprocessor.Render();

		ImGui::EndChild();
	}
	void ProjectSettingsPanel::RenderNavbar()
	{
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImVec2 size = ImGui::GetContentRegionAvail();

		ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.BackgroundColor);

		ImUI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2({ 8, 8 }));
		ImGui::BeginChild("##navbar", { 225, size.y }, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			float width = ImGui::GetContentRegionAvail().x;

			ImUI::ScopedStyleVar alignment(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5));

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImUI::MenuHeader("General");
			ImGui::PopFont();

			ImGui::Button("Application properties", { width, 32 });
			ImGui::Button("Input settings", { width, 32 });


			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImUI::MenuHeader("Runtime");
			ImGui::PopFont();
			ImGui::Button("Build settings", { width, 32 });

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImUI::MenuHeader("Rendering");
			ImGui::PopFont();

			ImGui::Button("Quality settings", { width, 32 });
			ImGui::Button("Lighting settings", { width, 32 });
			ImGui::Button("Shadow settings", { width, 32 });

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImUI::MenuHeader("Physics");
			ImGui::PopFont();

			ImGui::Button("Physics properties", { width, 32 });
			ImGui::Button("Collision layers", { width, 32 });

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImUI::MenuHeader("Scripting");
			ImGui::PopFont();

			ImGui::Button("Execution order", { width, 32 });
		}
		ImGui::EndChild();
	}
	void ProjectSettingsPanel::DrawInputSettingsPanel()
	{
		ImGui::Text("Here we have input settings");
	}
}