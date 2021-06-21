#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

namespace WindowElement {


	class FileView : public EditorWindow {
	public:
		FileView();
		~FileView();
		void Init() override;

		void OnWindowRender() override;
		void OnBeforeRender() override;
		void SetRootPath(const char* path);
		const std::string& GetRootPath() { return m_RootPath; }
		const std::string& GetCurrentPath() { return m_CurrentPath; }
		void UpdateFolderData();

	private:
		bool m_Open = true;
		float m_Scale = 100.0f;

		void DrawFolderTree();
		void DrawFilePath();
		void LoadAllFiles(const FolderData& data);
		void LoadFile(const std::string& file);
		ImVec4 GetFileColor(const std::string& type);

		Rendering::Texture2D* GetImageFor(const std::string& file);

		std::string m_RootPath;
		std::string m_CurrentPath;
		std::string m_SearchValue;

		FolderData m_FolderData;
	};
}