#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

namespace WindowElement {

	struct FolderViewData {
		std::string path;
		std::string name;
		std::vector<FolderViewData> subfolders;
		std::vector<std::filesystem::directory_entry> files;
	};


	class FileView : public EditorWindow {
	public:
		FileView();
		~FileView();

		void OnWindowRender() override;
		void SetRootPath(const char* path);
		const char* GetRootPath() { return rootPath.c_str(); }

		void UpdateFileTree();

	private:
		std::vector<FolderViewData> UpdateFolderFiles(FolderViewData& parent);

		void DrawFileTree(FolderViewData& data);
		void DrawFolder(FolderViewData& data);
		void DrawFile(const char* file);

		std::string rootPath;
		FolderViewData currentFolder;

		FolderViewData folderData;
	};
}