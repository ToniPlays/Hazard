#pragma once

#include "GUI/EditorWindow.h"

namespace WindowElement {
	class FileView : public EditorWindow {
	public:
		FileView();
		~FileView();

		void OnWindowRender() override;
		void SetRootPath(const char* rootPath) { this->rootPath = std::string(rootPath); currentPath = rootPath; }
		const char* GetRootPath() { return rootPath.c_str(); }
	private:
		std::string rootPath;
		std::string currentPath;
	};
}