#pragma once

#include "Hazard.h"
#include "Project/ProjectManager.h"
#include "AssetPanelItem.h"

namespace UI {

	struct FolderStructureData 
	{
		std::filesystem::path Path;
		std::vector<FolderStructureData> SubFolders;
	};

	class AssetPanel : public Hazard::ImUI::Panel {
	public:
		AssetPanel();

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;
		bool OnWindowFocus(WindowFocusEvent& e);

		void SetRootFolder(const std::filesystem::path& path) { 
			m_RootPath = path;
			m_CurrentPath = path;

			m_FolderData = GenerateFolderStructure();
			RefreshFolderItems();
		};

		void RefreshFolderItems();
		std::filesystem::path& GetRootDirectory() { return m_RootPath; }
		std::filesystem::path& GetOpenDirectory() { return m_CurrentPath; }

		void OpenImport();

	private:
		void DrawToolbar();
		void DrawFolderTreeView();
		void DrawContents();
		void DrawContextMenu();
		void DrawCurrentFolderPath();
		void DrawFolderTreeItem(const FolderStructureData& folder);
		Ref<Hazard::Texture2DAsset> GetItemIcon(const Hazard::AssetMetadata& metadata);

		std::vector<FolderStructureData> GenerateFolderStructure();
		std::vector<FolderStructureData> GenerateSubFolderData(const std::filesystem::path& folder);
		
		void GoToFolderDepth(const std::filesystem::path& path, uint32_t index);
	private:
		std::filesystem::path m_RootPath;
		std::filesystem::path m_CurrentPath;
		std::vector<FolderStructureData> m_FolderData;
		std::vector<AssetPanelItem> m_CurrentItems;

		std::vector<Ref<Hazard::Texture2DAsset>> m_Textures;
	};
}