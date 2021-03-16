#pragma once

#include <hzreditor.h>
#include "FileView.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Layout/ContextMenus.h"

using namespace WindowLayout;

namespace WindowElement {

	float windowElementWidth;

	constexpr float buttonWidth = 54;

	FileView::FileView() : EditorWindow(ICON_FK_FOLDER_OPEN" Project") 
	{
		
	}

	FileView::~FileView()
	{

	}
	void FileView::OnWindowRender()
	{
		using namespace Appereance;
		Color onColor = Color::FromHex("#404040");
		Color offColor = Style::GetStyleColor(ColorType::Secondary);

		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_FOLDER, offColor, onColor, Style::GetStyleColor(ColorType::Info), false, { 24, 24 })) {}
		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_ADDRESS_BOOK, offColor, onColor, Style::GetStyleColor(ColorType::Debug), false, { 24, 24 })) {}
		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_GLOBE, offColor, onColor, Style::GetStyleColor(ColorType::Error), false, { 24, 24 })) {}
		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_REFRESH, offColor, onColor, Style::GetStyleColor(ColorType::Warning), false, { 24, 24 })) {
			UpdateFileTree();
		}
		Layout::SameLine(0, 25);
		Layout::Text(currentFolder.path.c_str());

		Layout::SameLine(0, 5);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 300);
		Layout::Text("Search");
		Layout::SameLine(0, 10);
		std::string value;
		if (Input::InputField(value)) {
			
		}
		Layout::Separator();
		Layout::Table(2, true);
		ImGui::BeginChild("Folders");
		DrawFileTree(folderData);
		ImGui::EndChild();

		Layout::TableNext();
		ImGui::BeginChild("FolderData");

		windowElementWidth = 0;

		for (FolderViewData& data : currentFolder.subfolders) {
			DrawFolder(data);
		}
		for (std::filesystem::directory_entry data : currentFolder.files) {
			DrawFile(data.path().filename().string().c_str());
		}
		
		ContextMenus::FileContextMenu();

		ImGui::EndChild();
		Layout::EndTable();
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
		Utility::FolderData data = Utility::File::GetFolderFiles(rootPath.c_str());

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
		for (std::filesystem::directory_entry folder : Utility::File::GetFolderFiles(parent.path.c_str()).folders) {
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
			HZR_WARN("Selected navigation path {0}", currentFolder.path);
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
		Input::IconButton(data.name.c_str(), data.name.c_str(), { buttonWidth, buttonWidth });

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
			currentFolder = data;
		}

		windowElementWidth += buttonWidth + 5;
		if (windowElementWidth + buttonWidth + 5 <= Layout::GetColumnWidth()) {
			ImGui::SameLine(0, 5);
		}
		else windowElementWidth = 0;
	}
	void FileView::DrawFile(const char* file)
	{
		Input::IconButton(file, file, { buttonWidth, buttonWidth });
		windowElementWidth += buttonWidth + 5;
		if (windowElementWidth + buttonWidth + 5 <= Layout::GetColumnWidth()) {
			ImGui::SameLine(0, 5);
		}
		else windowElementWidth = 0;
	}
}