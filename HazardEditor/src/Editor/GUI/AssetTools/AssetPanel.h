#pragma once

#include "Hazard.h"
#include "Project/ProjectManager.h"
#include "AssetPanelItem.h"
#include "Hazard/ImGUI/UIElements/TextField.h"
#include "Hazard/ImGUI/Panel.h"

namespace UI
{

	enum AssetPanelFlags : uint32_t
	{
		AssetPanelFlags_SettingsOpen = BIT(0)
	};

	struct FolderStructureData
	{
		std::filesystem::path Path;
		std::vector<FolderStructureData> SubFolders;
	};

	class AssetPanel : public Hazard::ImUI::Panel
	{
	public:
		AssetPanel();

		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;
		bool OnWindowFocus(WindowFocusEvent& e);

		void SetSelectedFolder(const std::filesystem::path& path);
		void SetPreviouslySelectedFolder(uint32_t index);
		void SetRootFolder(const std::filesystem::path& path)
		{
			m_RootPath = path;

			m_PathSelection.clear();
			m_CurrentPathSelection = 0;

			SetSelectedFolder(path);
		};
		void Refresh();
		void RefreshFolderItems();

		std::filesystem::path& GetRootDirectory()
		{
			return m_RootPath;
		}
		std::filesystem::path& GetOpenDirectory()
		{
			return m_CurrentPath;
		}

		void OpenImport();

	private:
		void DrawToolbar();
		void DrawSettings();
		void DrawFolderTreeView();
		void DrawContents();
		void DrawContextMenu();
		void DrawCurrentFolderPath();

		void DrawFolderTreeItem(const FolderStructureData& folder);
		Ref<Hazard::Texture2DAsset> GetItemIcon(const Hazard::AssetMetadata& metadata);

		std::vector<FolderStructureData> GenerateFolderStructure();
		std::vector<FolderStructureData> GenerateSubFolderData(const std::filesystem::path& folder);

		void GoToFolderDepth(uint32_t index);

	private:
		void CreateFolder(const std::filesystem::path& path);

	private:
		std::filesystem::path m_RootPath;
		std::filesystem::path m_CurrentPath;

		std::vector<FolderStructureData> m_FolderData;
		std::vector<AssetPanelItem> m_CurrentItems;

		std::vector<std::filesystem::path> m_Paths;
		std::vector<std::filesystem::path> m_PathSelection;
		uint64_t m_CurrentPathSelection = 0;

		std::vector<Ref<Hazard::Texture2DAsset>> m_Textures;
		std::unordered_map<AssetHandle, Ref<Hazard::Texture2DAsset>> m_Icons;

		Hazard::ImUI::TextField m_SearchField;

		float m_Scale = 100.0f;
		uint32_t m_Flags = 0;
	};
}
