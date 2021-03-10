#pragma once

#include <hzreditor.h>
#include "FileView.h"
#include "GUI/Library/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Input.h"

using namespace WindowLayout;

namespace WindowElement {

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
		Layout::Table(2, false);
		Layout::SetColumnWidth(250);

		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_FOLDER, offColor, onColor, Style::GetStyleColor(ColorType::Info), true, { 28, 28 })) {}
		Layout::SameLine(0, 5);
		if (Input::ButtonColorChange(ICON_FK_ADDRESS_BOOK, offColor, onColor, Style::GetStyleColor(ColorType::Debug), true, { 28, 28 })) {}

		DrawFileTree(folderData);

		Layout::TableNext();
		ImGui::BeginChild("FolderData");

		for (std::filesystem::directory_entry data : folderData.files) {
			ImGui::Text(data.path().filename().string().c_str());
		}

		ImGui::EndChild();
		Layout::EndTable();
	}
	void FileView::SetRootPath(const char* path)
	{
		rootPath = (std::string(path) + "\\assets").c_str();
		UpdateFileTree();
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
		Layout::Treenode(data.name.c_str(), Appereance::Style::GetTreeNodeDefaultFlags(), [&]() {
			for (FolderViewData subData : data.subfolders) {
				DrawFileTree(subData);
			}
		});
	}
}