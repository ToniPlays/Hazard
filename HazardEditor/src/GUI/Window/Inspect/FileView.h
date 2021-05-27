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
		void SetRootPath(const char* path);
		const char* GetRootPath() { return m_RootPath.c_str(); }
		void UpdateFolderData();

	private:
		void DrawFolderTree();
		void DrawFilePath();
		Rendering::Texture2D* GetImageFor(std::string file);

		std::string m_RootPath;
		std::string m_CurrentPath;

		Ref<Hazard::Rendering::Texture2D> m_FolderImage;
		Ref<Hazard::Rendering::Texture2D> m_Image;

		FolderData m_FolderData;
	};
}