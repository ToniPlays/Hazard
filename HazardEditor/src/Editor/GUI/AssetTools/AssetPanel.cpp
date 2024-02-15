
#include "AssetPanel.h"
#include "Core/EditorAssetManager.h"
#include "Editor/EditorScriptManager.h"
#include "Editor/GUI/AllPanels.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/ImGUI/UIElements/Treenode.h"

#include "Core/Defines.h"

#include "imgui.h"
#include <Directory.h>
#include <Platform/OS.h>
#include <Editor/EditorWorldManager.h>
#include <Hazard/ImGUI/GUIManager.h>
#include <Hazard/Rendering/Mesh/MaterialAssetLoader.h>

namespace UI
{
	AssetPanel::AssetPanel() : Panel("Asset panel", false)
	{
		SetRootFolder(ProjectManager::GetAssetFolder());

		m_SearchField = ImUI::TextField("");
		m_SearchField.SetIcon((const char*)ICON_FK_SEARCH);
		m_SearchField.SetHint("Search...");
	}

	void AssetPanel::Update()
	{

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
	void AssetPanel::SetSelectedFolder(const std::filesystem::path& path)
	{
		if (path == m_CurrentPath) return;

		Application::Get().SubmitMainThread([&, folder = path]() {
			m_CurrentPath = folder;

			if (m_PathSelection.size() <= m_CurrentPathSelection)
				m_PathSelection.resize(m_CurrentPathSelection + 1);
			else
				m_PathSelection.erase(std::next(m_PathSelection.begin(), m_CurrentPathSelection + 1), m_PathSelection.end()); //Remove all previous values

			m_PathSelection[m_CurrentPathSelection] = folder;
			m_CurrentPathSelection++;
			Refresh();
		});
	}
	void AssetPanel::SetPreviouslySelectedFolder(uint32_t index)
	{
		if (m_PathSelection[index - 1] == m_CurrentPath) return;

		Application::Get().SubmitMainThread([&, index]() {
			m_CurrentPath = m_PathSelection[index - 1];
			m_CurrentPathSelection = index;
			Refresh();
		});
	}
	void AssetPanel::OpenImport()
	{
		std::filesystem::path file = File::OpenFileDialog();
		if (file.empty()) return;

		auto& panel = Application::Get().GetModule<Hazard::GUIManager>().GetExistingOrNew<AssetImporterPanel>();
		panel.Open(file);
	}
	void AssetPanel::DrawToolbar()
	{
		HZR_PROFILE_FUNCTION();
		float fontSize = ImGui::GetFontSize();

		const ImVec2 size = { ImGui::GetContentRegionAvail().x, fontSize + 16 };
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImUI::ScopedStyleStack frame(ImGuiStyleVar_FrameBorderSize, 0.0f, ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImUI::ScopedStyleColor childBg(ImGuiCol_ChildBg, style.BackgroundColor);
		ImGui::BeginChild("Toolbar", size);
		{
			ImUI::ScopedColorStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);
			if (ImGui::Button((const char*)ICON_FK_PLUS " ADD", { 75.0, fontSize + 12 }))
			{

			}
		}
		ImGui::SameLine(0, 5);
		if (ImGui::Button((const char*)ICON_FK_EXTERNAL_LINK_SQUARE " Import", { 75.0, fontSize + 12 }))
		{
			OpenImport();
		}
		ImGui::SameLine(0, 5);
		if (ImGui::Button((const char*)ICON_FK_FILE_TEXT " Save all", { 75.0, fontSize + 12 }))
		{

		}

		ImGui::SameLine(0, 15);

		if (m_CurrentPathSelection <= 1)
			ImGui::BeginDisabled();

		if (ImGui::Button((const char*)ICON_FK_ARROW_CIRCLE_O_LEFT, { fontSize + 12, fontSize + 12 }))
			SetPreviouslySelectedFolder(m_CurrentPathSelection - 1);

		if (m_CurrentPathSelection <= 1)
			ImGui::EndDisabled();

		if (m_PathSelection.size() <= m_CurrentPathSelection + 1)
			ImGui::BeginDisabled();

		ImGui::SameLine(0, 0);
		if (ImGui::Button((const char*)ICON_FK_ARROW_CIRCLE_O_RIGHT, { fontSize + 12, fontSize + 12 }))
		{
			SetPreviouslySelectedFolder(m_CurrentPathSelection + 1);
		}
		if (m_PathSelection.size() <= m_CurrentPathSelection + 1)
			ImGui::EndDisabled();

		ImGui::SameLine(0, 5);
		DrawCurrentFolderPath();

		ImGui::SameLine();
		ImGui::SetCursorPosX(size.x - 100.0);

		if (ImGui::Button((const char*)ICON_FK_COG " Settings", { 96.0, fontSize + 12 }))
			m_Flags |= AssetPanelFlags_SettingsOpen;

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
				ImUI::ShiftY(4.0f);
				m_SearchField.Render();
				ImUI::ShiftY(4.0f);

				ImUI::ScopedStyleStack vars(ImGuiStyleVar_FrameRounding, 0);
				ImUI::Treenode favorites("Favorites", true);
				favorites.Content([&]() {
					ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.Frame.FrameColor);
				});

				ImUI::Treenode folders(ProjectManager::GetProjectName(), true);
				folders.DefaultOpen();
				folders.Content([&]() {
					for (const auto& folder : m_FolderData)
						DrawFolderTreeItem(folder);
				});
				favorites.Render();
				folders.Render();

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
		float thumbailSize = m_Scale * 1.3f;
		const float paddingForOutline = 2.0f;
		const float scrollBarrOffset = 20.0f + ImGui::GetStyle().ScrollbarSize;
		float fontSize = ImGui::GetFontSize();

		ImVec2 size = ImGui::GetContentRegionAvail();
		float panelWidth = size.x - scrollBarrOffset;
		float cellSize = thumbailSize + 2.0f + paddingForOutline;
		int columnCount = Math::Max((int)(panelWidth / cellSize), 1);

		size.y -= (fontSize + 16);
		ImGui::BeginChild("#contentPanel", size);
		ImUI::ShiftY(8.0f);
		ImGui::Columns(columnCount, 0, false);

		for (auto& item : m_CurrentItems)
		{
			const AssetMetadata& metadata = item.GetMetadata();
			Ref<Texture2DAsset> itemIcon = m_Icons[metadata.Handle];

			if (!itemIcon) continue;

			item.BeginRender();
			item.OnRender(itemIcon, itemIcon->GetSampler(), thumbailSize);
			item.EndRender();
		}

		ImGui::Columns();
		DrawContextMenu();
		ImGui::EndChild();

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImUI::ScopedStyleColor childBg(ImGuiCol_ChildBg, style.BackgroundColor);

		ImGui::BeginChild("Info", { size.x, (fontSize + 12) });
		ImUI::Shift(4.0f, ((fontSize + 12) - ImGui::GetTextLineHeight()) * 0.5f);
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
				auto& panel = Application::Get().GetModule<GUIManager>().GetExistingOrNew<ScriptCreatePanel>();
				panel.SetDirectory(GetOpenDirectory());
				panel.Open();
			});
			ImUI::MenuItem("World", [&]() {
				Ref<World> world = AssetManager::CreateAsset(AssetType::World, CreateAssetSettings()).As<World>();
				Editor::EditorWorldManager::SetWorld(world);
				changed = true;
			});
			ImUI::MenuItem("Material", [&]() {
				
				MaterialAssetLoader::CreateSettings matSettings = {};
				CreateAssetSettings settings = {
					.SourcePath = "",
					.Settings = &matSettings
				};

				Ref<Material> material = AssetManager::CreateAsset(AssetType::Material, settings).As<Material>();

				SaveAssetSettings saveSettings = {
					.TargetPath = File::FindAvailableName(m_CurrentPath, "New material", "hasset"),
					.Flags = ASSET_MANAGER_COMBINE_ASSET | ASSET_MANAGER_SAVE_AND_UPDATE,
				};

				AssetManager::SaveAsset(material, saveSettings);
				changed = true;
			});

			ImUI::MenuHeader("Advanced assets");

			ImUI::Submenu("Animation", nullptr);
			ImUI::Submenu("Scripts", [&]() {
				ImUI::MenuItem("Entity script", [&]() {
					auto& panel = Application::Get().GetModule<GUIManager>().GetExistingOrNew<ScriptCreatePanel>();
					panel.SetDirectory(GetOpenDirectory());
					panel.Open();
				});
			});
			ImUI::Submenu("Editor", nullptr);
			ImUI::Submenu("Materials and textures", [&]() {
				ImUI::MenuItem("Material", nullptr);
				ImUI::MenuItem("Shader", nullptr);
				ImUI::MenuItem("Environment map", [&]() {
					//AssetManager::CreateNewAsset(AssetType::EnvironmentMap, path, File::Relative(m_RootPath, path));
					changed = true;
				});
			});
			ImUI::Submenu("Physics", nullptr);
			ImUI::Submenu("Sounds", nullptr);
			ImUI::Submenu("User interface", nullptr);

			ImUI::MenuHeader("Other");
			ImUI::MenuItem(LBL_SHOW_IN_EXPLORER, [&]() {
				OS::OpenDirectory(m_CurrentPath);
			});
		});

		if (!changed) return;

		Application::Get().SubmitMainThread([&]() {
			GenerateFolderStructure();
			RefreshFolderItems();
		});
	}

	void AssetPanel::Refresh()
	{
		m_FolderData = GenerateFolderStructure();
		RefreshFolderItems();
	}

	void AssetPanel::RefreshFolderItems()
	{
		std::vector<AssetPanelItem> directories;
		std::vector<AssetPanelItem> files;
		std::unordered_map<AssetHandle, Ref<Hazard::Texture2DAsset>> icons;

		for (auto& item : Directory::GetAllInDirectory(m_CurrentPath))
		{
			if (File::GetFileExtension(item) == ".hasset")
			{
				AssetHandle handle = AssetManager::AssetHandleFromFile(item);
				AssetPanelItem assetItem = AssetPanelItem(handle, File::GetPathNoExt(item));
				files.push_back(assetItem);
			}
			else if (File::IsDirectory(item))
			{
				AssetPanelItem folder(INVALID_ASSET_HANDLE, item);
				directories.push_back(folder);
			}
		}

		m_CurrentItems.clear();

		AssetHandle handle = EditorAssetManager::GetIconHandle("Folder");
		icons[0] = AssetManager::GetAsset<Texture2DAsset>(handle);

		for (auto& dir : directories)
			m_CurrentItems.push_back(dir);

		for (auto& f : files)
		{
			icons[f.GetHandle()] = GetItemIcon(f.GetMetadata());
			m_CurrentItems.push_back(f);
		}

		m_Icons = icons;
		m_Paths.clear();

		auto path = m_CurrentPath;

		while (path != m_RootPath && path.has_root_directory())
		{
			m_Paths.push_back(path);
			path = path.parent_path();
		}
	}

	void AssetPanel::DrawCurrentFolderPath()
	{
		HZR_PROFILE_FUNCTION();
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		float fontSize = ImGui::GetFontSize();

		if (ImGui::Button((const char*)ICON_FK_FOLDER " Content", { 96.0f, fontSize + 12 }))
		{
			SetSelectedFolder(m_RootPath);
		}
		for (uint32_t i = 0; i < (uint32_t)AssetType::Last; i++)
		{
			ImUI::DropTarget<AssetHandle>((AssetType)i, [&](AssetHandle handle) {
				//EditorAssetManager::MoveAssetToFolder(handle, m_RootPath);
				Refresh();
			});
		}

		for (uint64_t i = m_Paths.size(); i > 0; i--)
		{
			const auto& path = m_Paths[i - 1];
			ImGui::SameLine(0.0f, 8.0f);
			ImGui::TextColored(style.Window.HeaderActive, (const char*)ICON_FK_CHEVRON_RIGHT);
			ImGui::SameLine(0.0f, 8.0f);

			if (ImGui::Button(File::GetName(path).c_str(), { 0, fontSize + 12 }))
				GoToFolderDepth(i - 1);

			for (uint32_t j = 0; j < (uint32_t)AssetType::Last; j++)
			{
				ImUI::DropTarget<AssetHandle>((AssetType)j, [&, path](AssetHandle handle) {
					//EditorAssetManager::MoveAssetToFolder(handle, path);
					Refresh();
				});
			}
		}
	}
	void AssetPanel::DrawFolderTreeItem(const FolderStructureData& folder)
	{
		ImUI::Treenode treenode(File::GetName(folder.Path).c_str(), false);
		treenode.DefaultOpen();
		treenode.Content([&]() {
			if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered())
			{
				SetSelectedFolder(folder.Path);
				Application::Get().SubmitMainThread([&]() {
					RefreshFolderItems();
				});
			}
			for (const auto& subfolder : folder.SubFolders)
				DrawFolderTreeItem(subfolder);
		});
		treenode.Render();

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
		AssetHandle handle = EditorAssetManager::GetIconHandle("Default");

		switch (metadata.Type)
		{
			case AssetType::EnvironmentMap:
			{
				break;
			}
			case AssetType::Image:
			{
				if (metadata.LoadState == LoadState::Loading)
					break;
				if (metadata.LoadState == LoadState::None)
				{
					JobPromise promise = AssetManager::GetAssetAsync(metadata.Handle);
					promise.Then([&](JobGraph&) {
						Application::Get().SubmitMainThread([&]() {
							Refresh();
						});
					});

					break;
				}

				handle = metadata.Handle;
				break;
			}
			case AssetType::Script:
				handle = EditorAssetManager::GetIconHandle("Script"); break;
			case AssetType::World:
				handle = EditorAssetManager::GetIconHandle("World"); break;
			default:
				handle = EditorAssetManager::GetIconHandle("Default"); break;

		}

		return AssetManager::GetAsset<Texture2DAsset>(handle);
	}

	std::vector<FolderStructureData> AssetPanel::GenerateFolderStructure()
	{
		std::vector<FolderStructureData> result;

		for (auto& folder : Directory::GetAllInDirectory(m_RootPath))
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

		for (auto& subfolder : Directory::GetAllInDirectory(folder))
		{
			if (!File::IsDirectory(subfolder)) continue;

			auto& data = result.emplace_back();
			data.Path = subfolder;
			data.SubFolders = GenerateSubFolderData(subfolder);
		}

		return result;
	}
	void AssetPanel::GoToFolderDepth(uint32_t index)
	{
		std::filesystem::path newPath = m_CurrentPath;

		for (uint32_t i = 0; i < index; i++)
			newPath = newPath.parent_path();

		SetSelectedFolder(newPath);
	}
	void AssetPanel::CreateFolder(const std::filesystem::path& path)
	{
		std::filesystem::path directoryPath = path;
		uint32_t suffix = 1;

		while (File::Exists(directoryPath))
			directoryPath = path.string() + std::to_string(suffix);

		Directory::Create(directoryPath);

		Refresh();
	}
}
