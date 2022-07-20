
#include "AssetPanel.h"
namespace UI
{
	void AssetPanel::OnPanelRender()
	{
		ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		DrawToolbar();
		ImGui::Columns(2, 0, true);
		DrawFolderTreeView();
		ImGui::NextColumn();
		DrawContents();
		ImGui::Columns();
	}
	void AssetPanel::DrawToolbar()
	{
		const ImVec2 size = { ImGui::GetContentRegionAvailWidth(), 28.0f };
		const Style& style = StyleManager::GetCurrent();
		ScopedStyleStack frame(ImGuiStyleVar_FrameBorderSize, 0.0f, ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::BeginChild("Toolbar", size);

		{
			ScopedColourStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);
			if (ImGui::Button(ICON_FK_PLUS " ADD", { 75.0, 28.0f })) {

			}
		}
		ImGui::SameLine(0, 5);
		if (ImGui::Button(ICON_FK_EXTERNAL_LINK_SQUARE " Import", { 75.0, 28.0f })) {

		}
		ImGui::SameLine(0, 5);
		if (ImGui::Button(ICON_FK_FILE_TEXT " Save all", { 75.0, 28.0f })) {

		}
		ImGui::SameLine(0, 15);
		if (ImGui::Button(ICON_FK_ARROW_CIRCLE_O_LEFT, { 28.0, 28.0f })) {

		}
		ImGui::SameLine(0, 0);
		if (ImGui::Button(ICON_FK_ARROW_CIRCLE_O_RIGHT, { 28.0, 28.0f })) {

		}

		ImGui::SameLine(0, 5);
		DrawCurrentFolderPath();


		ImGui::SameLine(size.x - 80, 0);
		if (ImGui::Button(ICON_FK_COG " Settings", { 75.0, 28.0f })) {

		}

		ImGui::EndChild();
		Separator({ size.x, 2.0f }, style.Frame.FrameColor);
	}
	void AssetPanel::DrawFolderTreeView()
	{
		const Style& style = StyleManager::GetCurrent();

		ScopedStyleStack vars(ImGuiStyleVar_FrameRounding, 0, ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		UI::Treenode("Favorites", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed, [&]() {
			ScopedStyleColor color(ImGuiCol_ChildBg, style.Frame.FrameColor);
				
			});
		UI::Treenode("ProjectNameHere", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed, [&]() {
				
			});
	}
	void AssetPanel::DrawContents()
	{

	}
	void AssetPanel::DrawCurrentFolderPath()
	{
		const Style& style = StyleManager::GetCurrent();
		std::string relative = m_CurrentPath.string().substr(m_RootPath.string().length());
		std::vector<std::string> paths = StringUtil::SplitString(relative, '\\');

		if (ImGui::Button(ICON_FK_FOLDER " Content", { 0, 28.0f })) {
			GoToFolderDepth(m_CurrentPath, 0);
		}

		for (uint32_t i = 0; i < paths.size(); i++) {
			const std::string& path = paths[i];
			ImGui::SameLine(0.0f, 8.0f);
			ImGui::TextColored(style.Window.HeaderActive, ICON_FK_CHEVRON_RIGHT);
			ImGui::SameLine(0.0f, 8.0f);
			if (ImGui::Button(path.c_str(), { 0, 28.0f })) {
				GoToFolderDepth(m_CurrentPath, i + 1);
			}
		}
	}
	void AssetPanel::GoToFolderDepth(const std::filesystem::path& path, uint32_t index)
	{
		std::string relative = m_CurrentPath.string().substr(m_RootPath.string().length());
		uint32_t strPos = StringUtil::OffsetOf(relative, '\\', index);
		relative = relative.substr(0, strPos);
		m_CurrentPath = m_RootPath.string() + '\\' + relative;

	}
}