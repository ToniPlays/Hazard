
#include "AssetPanel.h"
#include "Core/EditorAssetManager.h"
#include "Editor/EditorScriptManager.h"
#include "Core/GUIManager.h"
#include "GUI/AllPanels.h"

#include "Core/Defines.h"

#include "imgui.h"

namespace UI
{
	AssetPanel::AssetPanel() : Panel("Asset panel")
	{
		SetRootFolder(ProjectManager::GetAssetFolder());
	}
	void AssetPanel::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();
		ImUI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		{
			ImUI::ScopedStyleVar spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			DrawToolbar();
		}
		{
			ImUI::ScopedStyleVar spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, 275);
			DrawFolderTreeView();
			ImGui::NextColumn();
		}
		DrawContents();
		ImGui::Columns();


		//if(m_Flags & AssetPanelFlags_SettingsOpen)
		//    DrawSettings();
	}
	bool AssetPanel::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<WindowFocusEvent>(BIND_EVENT(AssetPanel::OnWindowFocus));
	}
	bool AssetPanel::OnWindowFocus(WindowFocusEvent& e)
	{
		GenerateFolderStructure();
		RefreshFolderItems();
		return false;
	}
	void AssetPanel::OpenImport()
	{
		std::filesystem::path file = File::OpenFileDialog();
		if (!file.empty())
		{
			auto panel = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<AssetImporterPanel>();
			panel->Open(file);
		}
	}
	void AssetPanel::DrawToolbar()
	{
		HZR_PROFILE_FUNCTION();
		const ImVec2 size = { ImGui::GetContentRegionAvail().x, 32.0f };
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImUI::ScopedStyleStack frame(ImGuiStyleVar_FrameBorderSize, 0.0f, ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImUI::ScopedStyleColor childBg(ImGuiCol_ChildBg, style.BackgroundColor);
		ImGui::BeginChild("Toolbar", size);
		{
			ImUI::ScopedColourStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);
			if (ImGui::Button((const char*)ICON_FK_PLUS " ADD", { 75.0, 28.0f })) {

			}
		}
		ImGui::SameLine(0, 5);
		if (ImGui::Button((const char*)ICON_FK_EXTERNAL_LINK_SQUARE " Import", { 75.0, 28.0f })) {
			OpenImport();
		}
		ImGui::SameLine(0, 5);
		if (ImGui::Button((const char*)ICON_FK_FILE_TEXT " Save all", { 75.0, 28.0f })) {

		}
		ImGui::SameLine(0, 15);
		if (ImGui::Button((const char*)ICON_FK_ARROW_CIRCLE_O_LEFT, { 28.0, 28.0f })) {

		}
		ImGui::SameLine(0, 0);
		if (ImGui::Button((const char*)ICON_FK_ARROW_CIRCLE_O_RIGHT, { 28.0, 28.0f })) {

		}

		ImGui::SameLine(0, 5);
		DrawCurrentFolderPath();


		ImGui::SameLine(size.x - 80, 0);
		if (ImGui::Button((const char*)ICON_FK_COG " Settings", { 75.0, 28.0f }))
		{
			m_Flags |= AssetPanelFlags_SettingsOpen;
		}
		ImGui::EndChild();
		ImUI::Separator({ size.x, 2.0f }, style.Frame.FrameColor);
	}
	void AssetPanel::DrawSettings()
	{
		ImGui::SetCursorPosX(12);
		ImGui::BeginChild("##settingsView", { 200, 160 }, false);

		ImGui::Dummy({ 0, 3 });

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 160);
		ImGui::SetColumnWidth(0, 100);
		ImGui::Text("Some text");
		ImGui::Columns();
		ImGui::EndChild();
	}

	void AssetPanel::DrawFolderTreeView()
	{
		HZR_PROFILE_FUNCTION();
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		{
			ImUI::ScopedStyleColor childBg(ImGuiCol_ChildBg, style.BackgroundColor);
			ImGui::BeginChild("##FileTree");
			{

				ImUI::ScopedStyleStack vars(ImGuiStyleVar_FrameRounding, 0, ImGuiStyleVar_FramePadding, ImVec2(4, 4));
				ImUI::Treenode("Favorites", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed, [&]() {
					ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.Frame.FrameColor);

					});
				ImUI::Treenode(ProjectManager::GetProjectName().c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen, [&]() {
					for (const auto& folder : m_FolderData) {
						DrawFolderTreeItem(folder);
					}
					});

				ImUI::ContextMenu([&]() {
					ImUI::MenuItem("Refresh", [&]() {
						m_FolderData = GenerateFolderStructure();
						});
					});
			}
		}
		ImGui::EndChild();
	}
	void AssetPanel::DrawContents()
	{
		HZR_PROFILE_FUNCTION();
		float thumbailSize = m_Scale;
		const float paddingForOutline = 2.0f;
		const float scrollBarrOffset = 20.0f + ImGui::GetStyle().ScrollbarSize;
		ImVec2 size = ImGui::GetContentRegionAvail();
		float panelWidth = size.x - scrollBarrOffset;
		float cellSize = thumbailSize + 2.0f + paddingForOutline;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1) columnCount = 1;

		size.y -= 32.0f;
		ImGui::BeginChild("#contentPanel", size);
		ImUI::ShiftY(8.0f);
		ImGui::Columns(columnCount, 0, false);

		for (auto& item : m_CurrentItems)
		{
			item.BeginRender();
			Ref<Texture2DAsset> itemIcon = item.IsFolder() ? EditorAssetManager::GetIcon("Folder") : GetItemIcon(item.GetMetadata());
			item.OnRender(itemIcon, thumbailSize);
			item.EndRender();
		}

		ImGui::Columns();
		DrawContextMenu();
		ImGui::EndChild();

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImUI::ScopedStyleColor childBg(ImGuiCol_ChildBg, style.BackgroundColor);
		ImGui::BeginChild("Info", { size.x, 28.0f });
		ImUI::Shift(4.0f, (28.0f - ImGui::GetTextLineHeight()) * 0.5f);
		ImGui::Text("Items: %zu", m_CurrentItems.size());

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);

		{
			ImUI::ScopedStyleVar framePadding(ImGuiStyleVar_FramePadding, ImVec2{ 2.0f, 2.0 });
			float width = ImGui::GetContentRegionAvail().x;
			ImUI::Shift(width - 100, -4.0f);
			ImGui::SliderFloat("##scale", &m_Scale, 80, 150);
		}

		ImGui::EndChild();
	}

	void AssetPanel::DrawContextMenu()
	{
		bool changed = false;

		ImUI::ContextMenu([&]() {
			ImUI::MenuHeader("Folder");
		ImUI::MenuItem("New folder", [&]() {
			CreateFolder(GetOpenDirectory() / "Folder");
		changed = true;
			});
		ImUI::MenuHeader("Import");
		ImUI::MenuItem("Import asset", [&]() {
			OpenImport();
			});

		ImUI::MenuHeader("Quick create");
		ImUI::MenuItem("Script", [&]() {
			auto panel = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<ScriptCreatePanel>();
		panel->SetDirectory(GetOpenDirectory());
		panel->Open();
			});
		ImUI::MenuItem("World", [&]() {
			CreateWorld(GetOpenDirectory() / "NewWorld");
		changed = true;
			});
		ImUI::MenuItem("Material", [&]() {
			//EditorAssetManager::CreateAsset(AssetType::Material, GetOpenDirectory() / "material.hmat");
			changed |= true;
			});

		ImUI::MenuHeader("Advanced assets");

		ImUI::Submenu("Animation", [&]() {

			});
		ImUI::Submenu("Scripts", [&]() {
			ImUI::MenuItem("Entity script", [&]() {
				ScriptCreatePanel* panel = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<ScriptCreatePanel>();
		panel->SetDirectory(GetOpenDirectory());
		panel->Open();
				});
			});
		ImUI::Submenu("Editor", [&]() {

			});
		ImUI::Submenu("Materials and textures", [&]() {

			ImUI::MenuItem("Material", [&]() {
				//EditorAssetManager::CreateAsset(AssetType::Material, GetOpenDirectory() / "newshader.glsl");
				changed |= true;
				});

		ImUI::MenuItem("Shader", [&]() {
			//EditorAssetManager::CreateAsset(AssetType::Shader, GetOpenDirectory() / "newshader.glsl");
			changed |= true;
			});
			});
		ImUI::Submenu("Physics", [&]() {

			});
		ImUI::Submenu("Sounds", [&]() {

			});
		ImUI::Submenu("User interface", [&]() {

			});

		ImUI::MenuHeader("Other");
		ImUI::MenuItem(LBL_SHOW_IN_EXPLORER, [&]() {
			File::OpenDirectoryInExplorer(m_CurrentPath);
			});
			});

		if (!changed) return;

		GenerateFolderStructure();
		RefreshFolderItems();
	}

	void AssetPanel::RefreshFolderItems()
	{
		std::vector<AssetPanelItem> directories;
		std::vector<AssetPanelItem> files;

		m_CurrentItems.clear();
		for (auto& item : File::GetAllInDirectory(m_CurrentPath))
		{
			AssetType type = Hazard::Utils::AssetTypeFromExtension(File::GetFileExtension(item));
			
			if (type != AssetType::Folder && type != AssetType::Undefined)
			{
				AssetHandle handle = AssetManager::GetHandleFromKey(item.lexically_normal().string());
				HZR_INFO("Updating file {} ({})", item.string(), handle);

				if (handle == INVALID_ASSET_HANDLE) continue;

				AssetPanelItem assetItem = AssetPanelItem(handle);
				files.push_back(assetItem);

			}
			else if (File::IsDirectory(item))
			{
				AssetPanelItem folder(item);
				directories.push_back(folder);
			}
		}

		for (auto& dir : directories)
			m_CurrentItems.push_back(dir);

		for (auto& f : files)
			m_CurrentItems.push_back(f);

		m_Paths.clear();

		auto path = m_CurrentPath;

		while (path != m_RootPath)
		{
			m_Paths.push_back(path);
			path = path.parent_path();
		}
	}

	void AssetPanel::DrawCurrentFolderPath()
	{
		HZR_PROFILE_FUNCTION();
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		if (ImGui::Button((const char*)ICON_FK_FOLDER " Content", { 0, 28.0f }))
		{
			GoToFolderDepth(m_CurrentPath, 0);
		}
		for (uint32_t i = 0; i < (uint32_t)AssetType::Last; i++)
		{
			ImUI::DropTarget<AssetHandle>((AssetType)i, [&](AssetHandle handle) {
				//EditorAssetManager::MoveAssetToFolder(handle, m_RootPath);
				Refresh();
				});
		}

		for (size_t i = m_Paths.size(); i > 0; i--) {

			const auto& path = m_Paths[i - 1];
			ImGui::SameLine(0.0f, 8.0f);
			ImGui::TextColored(style.Window.HeaderActive, (const char*)ICON_FK_CHEVRON_RIGHT);
			ImGui::SameLine(0.0f, 8.0f);

			if (ImGui::Button(File::GetName(path).c_str(), { 0, 28.0f }))
				GoToFolderDepth(m_CurrentPath, (uint32_t)i + 1);

			for (uint32_t i = 0; i < (uint32_t)AssetType::Last; i++)
			{
				ImUI::DropTarget<AssetHandle>((AssetType)i, [&, path](AssetHandle handle) {
					//EditorAssetManager::MoveAssetToFolder(handle, path);
					Refresh();
					});
			}
		}
	}
	void AssetPanel::DrawFolderTreeItem(const FolderStructureData& folder)
	{
		ImUI::Treenode(File::GetName(folder.Path).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth, [&]() {
			if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered())
			{
				m_CurrentPath = folder.Path;
				RefreshFolderItems();
			}
		for (const auto& subfolder : folder.SubFolders)
		{
			DrawFolderTreeItem(subfolder);
		}
			});

		for (uint32_t i = 0; i < (uint32_t)AssetType::Last; i++)
		{
			ImUI::DropTarget<AssetHandle>((AssetType)i, [&, path = folder.Path](AssetHandle handle) {
				//EditorAssetManager::MoveAssetToFolder(handle, path);
				Refresh();
				});
		}
	}

	Ref<Texture2DAsset> AssetPanel::GetItemIcon(const AssetMetadata& metadata)
	{

		switch (metadata.Type)
		{
		case AssetType::Image:
		{
			if (metadata.LoadState == LoadState::Loading)
				break;
			if (metadata.LoadState == LoadState::None)
			{
				JobPromise<Ref<Asset>> asset = AssetManager::GetAssetAsync<Asset>(metadata.Handle);
				break;
			}
			Ref<Texture2DAsset> asset = AssetManager::GetAsset<Texture2DAsset>(metadata.Handle);
			if (asset)
				return asset;
			break;
		}
		case AssetType::Script:
			return EditorAssetManager::GetIcon("Script");
		case AssetType::World:
			return EditorAssetManager::GetIcon("World");
		default:
			return EditorAssetManager::GetIcon("Default");
		}
		return EditorAssetManager::GetIcon("Default");
	}

	std::vector<FolderStructureData> AssetPanel::GenerateFolderStructure()
	{
		std::vector<FolderStructureData> result;

		for (auto& folder : File::GetAllInDirectory(m_RootPath))
		{
			if (!File::IsDirectory(folder)) continue;

			auto& data = result.emplace_back();
			data.Path = folder;
			data.SubFolders = GenerateSubFolderData(folder);
		}

		return result;
	}
	std::vector<FolderStructureData> AssetPanel::GenerateSubFolderData(const std::filesystem::path& folder)
	{
		std::vector<FolderStructureData> result;

		for (auto& subfolder : File::GetAllInDirectory(folder))
		{
			AssetHandle handle = AssetManager::GetHandleFromKey(subfolder.string());
			if (handle == INVALID_ASSET_HANDLE) continue;

			if (!File::IsDirectory(subfolder)) continue;

			auto& data = result.emplace_back();
			data.Path = subfolder;
			data.SubFolders = GenerateSubFolderData(subfolder);
		}

		return result;
	}
	void AssetPanel::GoToFolderDepth(const std::filesystem::path& path, uint32_t index)
	{
		std::filesystem::path newPath = m_CurrentPath;

		for (uint32_t i = 0; i < index + 1; i++)
			newPath = newPath.parent_path();

		m_CurrentPath = newPath;
		RefreshFolderItems();
	}
	void AssetPanel::CreateFolder(const std::filesystem::path& path)
	{
		std::filesystem::path directoryPath = path;
		uint32_t suffix = 1;

		while (File::Exists(directoryPath))
			directoryPath = path.string() + std::to_string(suffix);

		File::CreateDir(directoryPath);
	}
	void AssetPanel::CreateWorld(const std::filesystem::path& path)
	{
		std::filesystem::path worldPath = path.string() + ".hazard";
		uint32_t suffix = 1;

		while (File::Exists(worldPath))
			worldPath = path.string() + std::to_string(suffix) + ".hazard";

		Ref<World> newWorld = Ref<World>::Create(path);

		Entity entity = newWorld->CreateEntity("Camera");
		newWorld->CreateEntity("Entity 1");
		entity.AddComponent<CameraComponent>();

		YAML::Emitter out;
		out << YAML::BeginMap;
		YamlUtils::Serialize(out, "UID", AssetHandle());
		YamlUtils::Serialize(out, "Type", AssetType::World);
		out << YAML::EndMap;
		File::WriteFile(worldPath.string() + ".meta", out.c_str());

		WorldSerializer serializer(newWorld);
		
		serializer.SerializeEditor(worldPath);
	}
}
