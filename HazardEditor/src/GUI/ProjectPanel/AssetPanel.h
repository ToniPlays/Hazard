#pragma once

#include "GUI/Core/Panel.h"
#include "Project/ProjectManager.h"

namespace UI {
	class AssetPanel : public Panel {
	public:
		AssetPanel() : Panel("AssetPanel") {
			m_RootPath = ProjectManager::GetProject().GetProjectData().ProjectDirectory;
			m_CurrentPath = std::filesystem::path(m_RootPath.string() + "\\Scripts\\Binaries");
		};

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };

	private:
		void DrawToolbar();
		void DrawFolderTreeView();
		void DrawContents();
		void DrawCurrentFolderPath();
		
		void GoToFolderDepth(const std::filesystem::path& path, uint32_t index);

	private:
		std::filesystem::path m_RootPath;
		std::filesystem::path m_CurrentPath;
	};
}