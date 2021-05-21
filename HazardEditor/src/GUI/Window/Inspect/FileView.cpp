#pragma once

#include <hzreditor.h>
#include "FileView.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Layout/ContextMenus.h"

using namespace WindowLayout;

namespace WindowElement {

	FileView::FileView() : EditorWindow(ICON_FK_FOLDER_OPEN" Project") 
	{
		
	}

	FileView::~FileView()
	{

	}
	void FileView::Init()
	{
		using namespace Hazard::Rendering;

		Texture2DCreateInfo info;
		info.filename = "res/icons/logo.png";
		m_Image = RenderUtils::Create<Texture2D>(info);

		info.filename = "res/icons/folder.png";

		m_FolderImage = RenderUtils::Create<Texture2D>(info);
	}
	void FileView::OnWindowRender()
	{
		using namespace Appereance;
		Color onColor = Color::FromHex("#404040");
		Color offColor = Style::GetStyleColor(ColorType::Secondary);
		Layout::SameLine(0, 5);

		if (Input::ButtonColorChange(ICON_FK_FOLDER, offColor, onColor, Style::GetStyleColor(ColorType::Info), false, { 24, 24 })) {}
		Layout::Tooltip("Create Folder");
		Layout::SameLine(0, 5);

		if (Input::ButtonColorChange(ICON_FK_ADDRESS_BOOK, offColor, onColor, Style::GetStyleColor(ColorType::Debug), false, { 24, 24 })) {}
		Layout::Tooltip("Create script");
		Layout::SameLine(0, 5);

		if (Input::ButtonColorChange(ICON_FK_GLOBE, offColor, onColor, Style::GetStyleColor(ColorType::Error), false, { 24, 24 })) {}
		Layout::Tooltip("Material");
		Layout::SameLine(0, 5);

		if (Input::ButtonColorChange(ICON_FK_REFRESH, offColor, onColor, Style::GetStyleColor(ColorType::Warning), false, { 24, 24 })) {
			UpdateFileTree();
		}

		Layout::Tooltip("Update");
		Layout::SameLine(0, 25);
		Layout::Text(currentFolder.path.c_str());

		Layout::SameLine(0, 5);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 420);
		ImGui::SetNextItemWidth(Layout::GetMaxWidth() - 20);
		std::string value;

		Input::InputField(value, "Search");
		Layout::SameLine();

		Appereance::Style::SetButtonColors("#DB3721", "#C3311D", "#A02818");
		Appereance::Style::SelectFont(1);

		if (Input::Button("X", { 20, 20 })) {
			value = "";
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		int cols = ImGui::GetContentRegionAvailWidth() / 75;
		
		if (cols != 0) {


			ImGui::BeginChild("FolderData", { float(cols * 75), 0 });
			ImGui::Columns(cols, "##files", false);

			for (FolderViewData& data : currentFolder.subfolders) {
				DrawFolder(data);
				ImGui::NextColumn();
			}

			for (std::filesystem::directory_entry data : currentFolder.files) {
				Input::FileButton(data.path().filename().string().c_str(), m_Image.Raw(), { 50, 50 });
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
		}
		ContextMenus::FileContextMenu();
		ImGui::EndChild();
	}
	void FileView::SetRootPath(const char* path)
	{
		rootPath = (std::string(path) + "\\assets").c_str();
		UpdateFileTree();
		currentFolder = folderData;
	}
	void FileView::UpdateFileTree()
	{
		folderData.files.clear();
		folderData.subfolders.clear();
		folderData.path = rootPath;
		folderData.name = "Assets";
		FolderData data = File::GetFolderFiles(rootPath.c_str());

		for (std::filesystem::directory_entry folder : data.folders) {
			FolderViewData subData;
			subData.path = folder.path().string();
			subData.name = folder.path().filename().string();
			subData.subfolders = UpdateFolderFiles(subData);
			folderData.subfolders.push_back(subData);
		}
		folderData.files = data.files;
	}
	std::vector<FolderViewData> FileView::UpdateFolderFiles(FolderViewData& parent)
	{
		std::vector<FolderViewData> result;
		for (std::filesystem::directory_entry folder : File::GetFolderFiles(parent.path.c_str()).folders) {
			FolderViewData subData;
			subData.path = folder.path().string();
			subData.name = folder.path().filename().string();
			subData.subfolders = UpdateFolderFiles(subData);
			result.emplace_back(subData);
		}
		return result;
	}
	void FileView::DrawFileTree(FolderViewData& data)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = Layout::Treenode(data.name.c_str(), flags);

		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered()) {
			currentFolder = data;
		}

		if (opened) {
			for (FolderViewData& subData : data.subfolders) {
				DrawFileTree(subData);
			}
			ImGui::TreePop();
		}
	}
	void FileView::DrawFolder(FolderViewData& data)
	{
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
			currentFolder = data;
		}
		Input::FileButton(data.name.c_str(), m_FolderImage.Raw(), { 50, 50 });
	}
}