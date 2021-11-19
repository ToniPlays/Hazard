#pragma once

#include "GUI/EditorView.h"
#include "FileView.h"
#include "Library/Layout/Layout.h"
#include "Library/Style.h"
#include "Library/Input.h"
#include "Library/Layout/ContextMenus.h"
#include "Library/DragDropUtils.h"

using namespace WindowLayout;

namespace WindowElement {

	FileView::FileView() : EditorWindow(ICON_FK_FOLDER_OPEN" Project")
	{

	}
	void FileView::Init()
	{
		AssetHandle handle = AssetManager::ImportAsset("icons/folder.png");
		m_Images["folder"] = AssetManager::GetAsset<Rendering::Texture2D>(handle);

		handle = AssetManager::ImportAsset("icons/logo.png");
		m_Images["logo"] = AssetManager::GetAsset<Rendering::Texture2D>(handle);

		handle = AssetManager::ImportAsset("icons/textureBG.png");
		m_Images["textureBG"] = AssetManager::GetAsset<Rendering::Texture2D>(handle);

		handle = AssetManager::ImportAsset("icons/csharp.png");
		m_Images["csharp"] = AssetManager::GetAsset<Rendering::Texture2D>(handle);

		handle = AssetManager::ImportAsset("icons/world.png");
		m_Images["world"] = AssetManager::GetAsset<Rendering::Texture2D>(handle);
	}
	void FileView::OnWindowRender()
	{
		ImGuiHoveredFlags flags = ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows;
		m_Open = ImGui::IsItemHovered() || ImGui::IsWindowHovered(flags);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
		Layout::SameLine(0, 5);

		Color bgColor = Color::FromHex("#404040");
		Color color = Style::GetStyleColor(ColorType::Info);

		if (Input::ButtonColorChange(ICON_FK_FOLDER, color, color, bgColor, false, { 24, 24 })) {}
		Layout::Tooltip("Create Folder");
		Layout::SameLine(0, 5);

		color = Style::GetStyleColor(ColorType::Debug);

		if (Input::ButtonColorChange(ICON_FK_ADDRESS_BOOK, color, color, bgColor, false, { 24, 24 })) {}
		Layout::Tooltip("Create script");
		Layout::SameLine(0, 5);

		color = Style::GetStyleColor(ColorType::Error);

		if (Input::ButtonColorChange(ICON_FK_GLOBE, color, color, bgColor, false, { 24, 24 })) {}
		Layout::Tooltip("Material");
		Layout::SameLine(0, 5);

		color = Style::GetStyleColor(ColorType::Warning);
		if (Input::ButtonColorChange(ICON_FK_REFRESH, color, color, bgColor, false, { 24, 24 })) {
			UpdateFolderData();
		}

		Layout::Tooltip("Refresh");
		Layout::SameLine(0, 25);

		DrawFilePath();
		ImGui::SameLine();

		float width = ImGui::GetWindowWidth();

		float maxWidth = width < 300 ? width : 300;
		ImGui::SetCursorPosX(width - maxWidth - 55);
		ImGui::SetNextItemWidth(maxWidth);
		Input::InputField(m_SearchValue, ICON_FK_SEARCH " Search");
		ImGui::SameLine();

		ImGui::SetCursorPosX(width - 50);
		ImGui::SetNextItemWidth(50);
		ImGui::DragFloat("##Scale", &m_Scale, 0.25f, 80.0f, 150.0f, "%.1f");

		Layout::Separator();
		ImGui::Columns(2);

		ImGui::BeginChild("##folderTree");
		DrawFolderTree();

		ImGui::EndChild();

		ImGui::NextColumn();

		float colWidth = 0.97f * m_Scale;
		float colHeight = 1.50f * m_Scale;
		int cols = ImGui::GetContentRegionAvailWidth() / colWidth;

		if (cols > 0)
		{
			DrawContentRows(colWidth, colHeight, cols);
		}
		ImGui::Columns();
		ImGui::PopStyleVar();

	}
	void FileView::SetRootPath(const std::string& path)
	{
		m_RootPath = (path + "\\assets").c_str();
		m_CurrentPath = m_RootPath;

		UpdateFolderData();
	}
	void FileView::DrawFolderTree()
	{
		ImGuiTreeNodeFlags flags = Style::GetTreeNodeDefaultFlags();
		Layout::Treenode("Assets", flags, [&]() {
			flags = ImGuiTreeNodeFlags_OpenOnArrow;
			for (std::filesystem::directory_entry folder : m_FolderData.Folders) {
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
				m_CurrentPath = std::move(foldePath);
				UpdateFolderData();
			}
		}
	}
	void FileView::DrawContentRows(float colWidth, float colHeight, int cols)
	{
		//TODO: Add alt scroll zooming

		ImGui::BeginChild("##list", { cols * colWidth, 0 }, false, ImGuiWindowFlags_NoScrollbar);
		ImGui::Columns(cols, "##files", false);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#323234")));
		ImGui::PushStyleColor(ImGuiCol_Button, Style::ColorAsImVec4(Color::FromHex("#181816")));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 5 });

		ImGui::PushStyleColor(ImGuiCol_Separator, Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Text)));

		for (std::filesystem::directory_entry folder : m_FolderData.Folders)
		{
			std::string name = folder.path().filename().string();
			Input::FileButton(name.c_str(), m_Images["folder"], [&]() {}, { colWidth - 5, colHeight });

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				m_CurrentPath = folder.path().string();
				UpdateFolderData();
			}
			ImGui::NextColumn();
		}
		ImGui::PopStyleColor();
		for (std::filesystem::directory_entry file : m_FolderData.Files)
		{
			std::filesystem::path relative = AssetManager::ToRelative(file.path());
			AssetHandle handle = AssetManager::GetHandleFromFile(relative.string());
			AssetMetadata& metadata = AssetManager::GetMetadata(handle);
			ImGui::PushStyleColor(ImGuiCol_Separator, GetFileColor(metadata));

			Input::FileButton(File::GetNameNoExt(metadata.Path.string()).c_str(), GetFileImageFromType(metadata), [&]() {
				DragDropUtils::DragSource(Hazard::Utils::AssetTypeToString(metadata.Type), metadata.Path.filename().string(), (AssetHandle*)&metadata.Handle);

				}, { colWidth - 5, colHeight });

			ImGui::PopStyleColor();
			ImGui::NextColumn();
		}
		
		ContextMenus::FileContextMenu(*this);
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();
		ImGui::EndChild();
	}
	ImVec4 FileView::GetFileColor(AssetMetadata& metadata)
	{
		switch (metadata.Type)
		{
		case Hazard::AssetType::Undefined:			return Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Warning));
		case Hazard::AssetType::AudioClip:			return Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Debug));
		case Hazard::AssetType::World:				return Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Primary));
		case Hazard::AssetType::Image:				return Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Critical));
		case Hazard::AssetType::EnvironmentMap:		return Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Primary));
		case Hazard::AssetType::PhysicsMaterial:	return Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Primary));
		}
	}
	Ref<Rendering::Texture2D> FileView::GetFileImageFromType(const AssetMetadata& metadata)
	{
		switch (metadata.Type)
		{
		case Hazard::AssetType::World:				return m_Images["world"];
		case Hazard::AssetType::Image: {
			Ref<Rendering::Image2D> asset = AssetManager::GetAsset<Rendering::Image2D>(metadata.Handle);
			return asset;
		}
		case Hazard::AssetType::AudioClip:
		case Hazard::AssetType::EnvironmentMap:
		case Hazard::AssetType::PhysicsMaterial:	
			return m_Images["folder"];
		}
		return m_Images["logo"];
	}
	void FileView::UpdateFolderData()
	{
		if (!m_RootPath.empty())
			m_FolderData = File::GetFolderFiles(m_CurrentPath);
	}
}