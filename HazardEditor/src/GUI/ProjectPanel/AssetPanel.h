#pragma once

#include "GUI/Core/Panel.h"
#include "Project/ProjectManager.h"
#include "AssetPanelItem.h"

namespace UI {

	struct FolderStructureData 
	{
		std::filesystem::path Path;
		std::vector<FolderStructureData> SubFolders;
	};

	class AssetPanel : public Panel {
	public:
		AssetPanel();

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };

		void SetRootFolder(const std::filesystem::path& path) { 
			m_RootPath = path;
			m_CurrentPath = path;

			m_FolderData = GenerateFolderStructure();
			RefreshFolderItems();
		};

		void RefreshFolderItems();

	private:
		void DrawToolbar();
		void DrawFolderTreeView();
		void DrawContents();
		void DrawCurrentFolderPath();
		void DrawFolderTreeItem(const FolderStructureData& folder);
		Ref<Texture2D> GetItemIcon(const AssetMetadata& metadata);

		std::vector<FolderStructureData> GenerateFolderStructure();
		std::vector<FolderStructureData> GenerateSubFolderData(const std::filesystem::path& folder);
		
		void GoToFolderDepth(const std::filesystem::path& path, uint32_t index);

	private:
		std::filesystem::path m_RootPath;
		std::filesystem::path m_CurrentPath;
		std::vector<FolderStructureData> m_FolderData;

		std::vector<AssetPanelItem> m_CurrentItems;
		std::unordered_map<AssetType, Ref<Texture2D>> m_Icons;
	};
}