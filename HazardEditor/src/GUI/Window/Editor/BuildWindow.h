#pragma once
#include "GUI/EditorWindow.h"
#include "BuildSystem/GameExportManager.h"

namespace WindowElement {

	class BuildWindow : public EditorWindow {
	public:
		BuildWindow();
		~BuildWindow() = default;

		void OnWindowRender() override;
		void ExportProject(const std::filesystem::path& path);
	private:
		void ShowBuildReport();
		void ShowBuildSettings();

		//Exporter::BuildReport m_Report;
		//Exporter::BuildSettings m_BuildSettings;
		bool m_HasBuilt = false;
	};
}