
#include "AssetPanel.h"
#include "Core/EditorAssetManager.h"
#include "Editor/EditorScriptManager.h"
#include "Core/GUIManager.h"

namespace UI
{
	AssetPanel::AssetPanel() : Panel("Asset panel")
	{
		{
			AssetHandle handle = AssetManager::ImportAsset("res/Icons/textureBG.png");
			m_Icons[AssetType::Undefined] = AssetManager::GetAsset<Texture2D>(handle);
		}
		{
			AssetHandle handle = AssetManager::ImportAsset("res/Icons/folder.png");
			m_Icons[AssetType::Folder] = AssetManager::GetAsset<Texture2D>(handle);
		}
		{
			AssetHandle handle = AssetManager::ImportAsset("res/Icons/textureBG.png");
			m_Icons[AssetType::AudioClip] = AssetManager::GetAsset<Texture2D>(handle);
		}
		{
			AssetHandle handle = AssetManager::ImportAsset("res/Icons/world.png");
			m_Icons[AssetType::World] = AssetManager::GetAsset<Texture2D>(handle);
		}
		{
			AssetHandle handle = AssetManager::ImportAsset("res/Icons/textureBG.png");
			m_Icons[AssetType::Image] = AssetManager::GetAsset<Texture2D>(handle);
		}
		{
			AssetHandle handle = AssetManager::ImportAsset("res/Icons/textureBG.png");
			m_Icons[AssetType::Mesh] = AssetManager::GetAsset<Texture2D>(handle);
		}
		{
			AssetHandle handle = AssetManager::ImportAsset("res/Icons/csharp.png");
			m_Icons[AssetType::Script] = AssetManager::GetAsset<Texture2D>(handle);
		}
		{
			AssetHandle handle = AssetManager::ImportAsset("res/Icons/csharp.png");
			m_Icons[AssetType::EnvironmentMap] = AssetManager::GetAsset<Texture2D>(handle);
		}
		{
			AssetHandle handle = AssetManager::ImportAsset("res/Icons/csharp.png");
			m_Icons[AssetType::PhysicsMaterial] = AssetManager::GetAsset<Texture2D>(handle);
		}


		SetRootFolder(ProjectManager::GetAssetFolder());
	}
	void AssetPanel::OnPanelRender()
	{
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
	void AssetPanel::DrawToolbar()
	{
		const ImVec2 size = { ImGui::GetContentRegionAvailWidth(), 32.0f };
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImUI::ScopedStyleStack frame(ImGuiStyleVar_FrameBorderSize, 0.0f, ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImUI::ScopedStyleColor childBg(ImGuiCol_ChildBg, style.BackgroundColor);
		ImGui::BeginChild("Toolbar", size);
		{
			ImUI::ScopedColourStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);
			if (ImGui::Button(ICON_FK_PLUS " ADD", { 75.0, 28.0f })) {

			}
		}
		ImGui::SameLine(0, 5);
		if (ImGui::Button(ICON_FK_EXTERNAL_LINK_SQUARE " Import", { 75.0, 28.0f })) {
			std::filesystem::path file = File::OpenFileDialog();
			if (!file.empty()) {
				File::Copy(file, m_CurrentPath / File::GetName(file), CopyOptions::UpdateExisting);
				File::NewFile(m_CurrentPath / (File::GetName(file) + ".meta"));
				Hazard::AssetManager::ImportAsset(m_CurrentPath / File::GetName(file));
				RefreshFolderItems();
			}
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
		ImUI::Separator({ size.x, 2.0f }, style.Frame.FrameColor);
	}
	void AssetPanel::DrawFolderTreeView()
	{

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
		constexpr float thumbailSize = 150.0f;
		const float paddingForOutline = 2.0f;
		const float scrollBarrOffset = 20.0f + ImGui::GetStyle().ScrollbarSize;
		ImVec2 size = ImGui::GetContentRegionAvail();
		float panelWidth = size.x - scrollBarrOffset;
		float cellSize = 150.0f + 2.0f + paddingForOutline;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1) columnCount = 1;

		size.y -= 32.0f;
		ImGui::BeginChild("#contentPanel", size);
		ImUI::ShiftY(8.0f);
		ImGui::Columns(columnCount, 0, false);

		for (auto& item : m_CurrentItems) {
			item.BeginRender();

			Ref<Texture2D> itemIcon = GetItemIcon(item.GetMetadata());
			item.OnRender(itemIcon, thumbailSize);
			item.EndRender();
		}
		ImGui::Columns();
		DrawContextMenu();
		ImGui::EndChild();

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImUI::ScopedStyleColor childBg(ImGuiCol_ChildBg, style.BackgroundColor);
		ImGui::BeginChild("Info", { size.x, 28.0f });

		ImGui::EndChild();
	}

	void AssetPanel::DrawContextMenu()
	{
		bool changed = false;
		ImUI::ContextMenu([&]() {

			ImUI::MenuHeader("Folder");
			ImUI::MenuItem("New folder", [&]() {
				EditorAssetManager::CreateFolder(GetOpenDirectory() / "Folder");
				});
			ImUI::MenuHeader("Import");
			ImUI::MenuItem("Import asset", [&]() {

				});


			ImUI::MenuHeader("Quick create");
			ImUI::MenuItem("Script", [&]() {
				//EditorAssetManager::CreateAsset(AssetType::Script, GetOpenDirectory() / "script.cs");
				//Application::GetModule<Editor::EditorScriptManager>().GenerateProjectFiles();
				});
			ImUI::MenuItem("World", [&]() {
				//EditorAssetManager::CreateAsset(AssetType::World, GetOpenDirectory() / "world.hazard");
				});
			ImUI::MenuItem("Material", [&]() {

				});

			ImUI::MenuHeader("Advanced assets");

			ImUI::Submenu("Animation", [&]() {

				});
			ImUI::Submenu("Scripts", [&]() {
				ImUI::MenuItem("Entity script", [&]() {
					//Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<ScriptCreatePanel>();
					});
				});
			ImUI::Submenu("Editor", [&]() {
				});
			ImUI::Submenu("Materials and textures", [&]() {

				});
			ImUI::Submenu("Physics", [&]() {

				});
			ImUI::Submenu("Sounds", [&]() {

				});
			ImUI::Submenu("User interface", [&]() {

				});

			ImUI::MenuHeader("Other");
			ImUI::MenuItem("Show in Explorer", [&]() {
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
		for (auto item : File::GetAllInDirectory(m_CurrentPath))
		{
			if (File::GetFileExtension(item) == "meta")
			{
				std::filesystem::path assetPath = File::GetPathNoExt(item);
				if (!File::Exists(assetPath)) continue;

				AssetHandle handle = AssetManager::GetHandleFromFile(assetPath.string());
				AssetPanelItem assetItem = AssetPanelItem(handle);

				File::IsDirectory(assetPath) ? directories.push_back(assetItem) : files.push_back(assetItem);
			}
		}

		for (auto& dir : directories) {
			m_CurrentItems.push_back(dir);
		}
		for (auto& f : files) {
			m_CurrentItems.push_back(f);
		}
	}

	void AssetPanel::DrawCurrentFolderPath()
	{
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
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
	void AssetPanel::DrawFolderTreeItem(const FolderStructureData& folder)
	{
		ImUI::Treenode(File::GetName(folder.Path).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth, [&]() {
			if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered()) {
				m_CurrentPath = folder.Path;
				RefreshFolderItems();
			}

			for (const auto& subfolder : folder.SubFolders) {
				DrawFolderTreeItem(subfolder);
			}
			});
	}

	Ref<Texture2D> AssetPanel::GetItemIcon(const AssetMetadata& metadata)
	{
		switch (metadata.Type)
		{
		case AssetType::Image:
			return AssetManager::GetAsset<Texture2D>(metadata.Handle);
		}
		return m_Icons[metadata.Type];
	}

	std::vector<FolderStructureData> AssetPanel::GenerateFolderStructure()
	{
		std::vector<FolderStructureData> result;

		for (auto folder : File::GetAllInDirectory(m_RootPath)) {
			AssetHandle handle = AssetManager::GetHandleFromFile(folder.string());
			if (handle == INVALID_ASSET_HANDLE) continue;

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

		for (auto subfolder : File::GetAllInDirectory(folder)) {
			AssetHandle handle = AssetManager::GetHandleFromFile(subfolder.string());
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
		std::string relative = m_CurrentPath.string().substr(m_RootPath.string().length());
		uint32_t strPos = StringUtil::OffsetOf(relative, '\\', index);
		relative = relative.substr(0, strPos);
		m_CurrentPath = m_RootPath.string() + '\\' + relative;

		RefreshFolderItems();
	}
}