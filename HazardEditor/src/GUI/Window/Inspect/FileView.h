#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

namespace WindowElement {

	class FileView : public EditorWindow {
	public:
		FileView();
		~FileView() = default;
		void Init() override;

		void OnWindowRender() override;
		void SetRootPath(const char* path);
		const std::string& GetRootPath() { return m_RootPath; }
		const std::string& GetCurrentPath() { return m_CurrentPath; }
		void UpdateFolderData();

	private:
		void DrawFolderTree();
		void DrawFilePath();
		Ref<Hazard::Rendering::Image2D> GetFileImageFromType(const Hazard::AssetMetadata& metadata);
		ImVec4 GetFileColor(Hazard::AssetMetadata& metadata);

		void DrawContentRows(float colWidth, float colHeight, int cols);
		bool m_Open = true;
		float m_Scale = 100.0f;

		std::string m_RootPath;
		std::string m_CurrentPath;
		std::string m_SearchValue;

		Hazard::FolderData m_FolderData;

		std::unordered_map<std::string, Ref<Hazard::Rendering::Image2D>> m_Images;
	};
}