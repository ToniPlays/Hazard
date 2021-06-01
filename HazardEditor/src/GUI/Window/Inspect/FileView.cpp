#pragma once

#include <hzreditor.h>
#include "FileView.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Layout/ContextMenus.h"
#include "GUI/Window/DragDropUtils.h"
#include <future>

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
		LoadFile("res/icons/folder.png");
		LoadFile("res/icons/logo.png");
		LoadFile("res/icons/textureBG.png");
		LoadFile("res/icons/csharp.png");
		LoadFile("res/icons/world.png");
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

		float width = ImGui::GetWindowWidth();
		
		float maxWidth = width < 300 ? width : 300;
		ImGui::SetCursorPosX(width - maxWidth - 55);
		ImGui::SetNextItemWidth(maxWidth);
		Input::InputField(m_SearchValue, ICON_FK_SEARCH " Search");
		ImGui::SameLine();

		ImGui::SetCursorPosX(width - 50);
		ImGui::SetNextItemWidth(50);
		ImGui::DragFloat("##Scale", &m_Scale,0.25f, 80.0f, 150.0f, "%.1f");

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
			ImGui::BeginChild("##list", { cols * colWidth, 0 }, false, ImGuiWindowFlags_NoScrollbar);
			ImGui::Columns(cols, "##files", false);

			Ref<Rendering::Texture2D> folderImage = Vault::Get<Rendering::Texture2D>("res/icons/folder.png");

			ImGui::PushStyleColor(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#323234")));
			ImGui::PushStyleColor(ImGuiCol_Button, Style::ColorAsImVec4(Color::FromHex("#181816")));

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 5 });

			ImGui::PushStyleColor(ImGuiCol_Separator, Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Text)));
			for (std::filesystem::directory_entry folder : m_FolderData.folders)
			{
				std::string name = folder.path().filename().string();
				Input::FileButton(name.c_str(), folderImage.Raw(), [&]() {}, { colWidth - 10, colHeight });

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
					m_CurrentPath = folder.path().string();
					UpdateFolderData();
				}
				ImGui::NextColumn();
			}
			ImGui::PopStyleColor();
			for (std::filesystem::directory_entry file : m_FolderData.files)
			{
				std::string name = file.path().filename().string();
				std::string type = DragDropUtils::TypeFromFile(name);
				Rendering::Texture2D* texture = GetImageFor(file.path().string());
				ImGui::PushStyleColor(ImGuiCol_Separator, GetFileColor(DragDropUtils::TypeFromFile(name)));
				Input::FileButton(File::GetNameNoExt(name).c_str(), texture, [&]() {

					DragDropUtils::DragSource(type.c_str(),
						name, file.path().string());

					}, { colWidth - 10, colHeight });

				ImGui::NextColumn();
				ImGui::PopStyleColor();
			}

			ContextMenus::FileContextMenu(*this);
			ImGui::PopStyleColor(2);
			ImGui::PopStyleVar();
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
		LoadAllFiles(m_FolderData);
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
	void FileView::LoadAllFiles(FolderData data)
	{
		for (std::filesystem::directory_entry folder : data.folders)
		{
			LoadAllFiles(File::GetFolderFiles(folder.path().string()));
		}
		for (std::filesystem::directory_entry file : data.files) {
			LoadFile(file.path().string());
		}
	}
	void FileView::LoadFile(const std::string& file)
	{
		std::string ext = File::GetFileExtension(file);
		if (ext == "jpg" || ext == "jpeg" || ext == "png")
		{
			Rendering::Texture2DCreateInfo info;
			info.filename = file;
			Rendering::RenderUtils::Create<Rendering::Texture2D>(info).Raw()->IncRefCount();
		}
	}
	ImVec4 FileView::GetFileColor(const std::string& type)
	{
		if (type == "Texture2D")	return Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Warning));
		if (type == "Script")	return Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Debug));
		if (type == "World")		return Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Primary));
		if (type == "Material")		return Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Critical));
		return Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Primary));
	}
	Rendering::Texture2D* FileView::GetImageFor(const std::string& file)
	{
		std::string ext = File::GetFileExtension(file);

		if (ext == "jpg" || ext == "jpeg" || ext == "png") {
			return Vault::Get<Rendering::Texture2D>(file.c_str());
		}
		if (ext == "cs") 
			return Vault::Get<Rendering::Texture2D>("res/icons/csharp.png");
		if (ext == "hazard") {
			return Vault::Get<Rendering::Texture2D>("res/icons/world.png");
		}
		return Vault::Get<Rendering::Texture2D>("res/icons/logo.png");
	}
	void FileView::UpdateFolderData()
	{
		if (m_RootPath == "")
			return;
		m_FolderData = File::GetFolderFiles(m_CurrentPath);
	}
}