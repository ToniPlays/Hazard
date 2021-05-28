#pragma once

#include <hzreditor.h>
#include "FileView.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Layout/ContextMenus.h"
#include "GUI/Window/DragDropUtils.h"

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
		info.filename = "res/icons/folder.png";
		m_FolderImage = RenderUtils::Create<Texture2D>(info);
		info.filename = "res/icons/logo.png";
		m_Image = RenderUtils::Create<Texture2D>(info);
	}
	void FileView::OnBeforeRender()
	{

	}
	void FileView::OnWindowRender()
	{
		ImGuiHoveredFlags flags = ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows;
		m_Open = ImGui::IsItemHovered() || ImGui::IsWindowHovered(flags);

		Color onColor = Color::FromHex("#404040");
		Color offColor = Style::GetStyleColor(ColorType::Secondary);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
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
			UpdateFolderData();
		}

		Layout::Tooltip("Refresh");
		Layout::SameLine(0, 25);

		DrawFilePath();
		ImGui::SameLine();
		float width = ImGui::GetContentRegionAvailWidth();
		float maxWidth = width < 200 ? width : 200;
		ImGui::SetCursorPosX(width - maxWidth);
		Input::InputField(m_SearchValue, ICON_FK_SEARCH " Search");

		Layout::Separator();
		ImGui::Columns(2);

		ImGui::BeginChild("##folderTree");

		DrawFolderTree();

		ImGui::EndChild();
		ImGui::NextColumn();

		int cols = ImGui::GetContentRegionAvailWidth() / 75;

		if (cols > 0) {
			ImGui::BeginChild("##list", { cols * 75.0f, 0 }, false, ImGuiWindowFlags_NoScrollbar);
			ImGui::Columns(cols, "##files", false);

			for (std::filesystem::directory_entry folder : m_FolderData.folders)
			{
				std::string name = folder.path().filename().string();
				Input::FileButton(name.c_str(), m_FolderImage.Raw(), { 50, 50 });

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
					m_CurrentPath = folder.path().string();
					UpdateFolderData();
				}
				ImGui::NextColumn();
			}
			char filePath[256];
			for (std::filesystem::directory_entry file : m_FolderData.files)
			{
				std::string name = file.path().filename().string();
				Rendering::Texture2D* texture = GetImageFor(file.path().string());
				Input::FileButton(name.c_str(), texture, { 50, 50 });

				memcpy(filePath, file.path().string().c_str(), sizeof(filePath));

				DragDropUtils::DragSource(DragDropUtils::TypeFromFile(name).c_str(), 
					file.path().filename().string(), filePath, sizeof(filePath));
				ImGui::NextColumn();
			}
			ImGui::EndChild();
		}
		ImGui::Columns();
		ImGui::PopStyleVar();
	}
	void FileView::SetRootPath(const char* path)
	{
		m_RootPath = (std::string(path) + "\\assets").c_str();
		m_CurrentPath = m_RootPath;
		UpdateFolderData();
	}
	void FileView::DrawFolderTree()
	{
		ImGuiTreeNodeFlags flags = Style::GetTreeNodeDefaultFlags();
		Layout::Treenode("Assets", flags, [&]() {
			flags = ImGuiTreeNodeFlags_OpenOnArrow;
			for (std::filesystem::directory_entry folder : m_FolderData.folders) {
				Layout::Treenode(folder.path().filename().string().c_str(), flags, [&]() {

				});
			}
			});
	}
	void FileView::DrawFilePath()
	{
		if (Input::Button("Assets")) {
			m_CurrentPath = m_RootPath;
			UpdateFolderData();
		}

		std::string relative = m_CurrentPath.substr(m_RootPath.size());
		std::vector<std::string> paths = StringUtil::SplitString(relative, '\\');

		for (size_t i = 0; i < paths.size(); i++) {
			ImGui::SameLine(0, 1);
			Layout::TextColored(ICON_FK_CHEVRON_RIGHT, Style::GetStyleColor(Primary));
			ImGui::SameLine(0, 1);

			if (Input::Button(paths[i].c_str())) {
				std::string foldePath = m_RootPath;
				for (size_t j = 0; j < i + 1; j++)
				{
					foldePath += "\\" + paths[i];
				}
				m_CurrentPath = foldePath;
				UpdateFolderData();
			}
		}
	}
	Rendering::Texture2D* FileView::GetImageFor(const std::string& file)
	{
		std::string extension = File::GetFileExtension(file);

		if (extension == "jpeg" || extension == "png") {
			return Vault::Get<Rendering::Texture2D>(file.c_str());
		}
		return m_Image.Raw();
	}
	void FileView::UpdateFolderData()
	{
		HZR_INFO("Selected path {0}", m_CurrentPath);
		if (m_RootPath == "") return;
		m_FolderData = File::GetFolderFiles(m_CurrentPath);
	}
}