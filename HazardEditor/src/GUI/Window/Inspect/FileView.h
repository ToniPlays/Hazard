#pragma once

#include "GUI/EditorWindow.h"

namespace WindowElement {
	class FileView : public EditorWindow {
	public:
		FileView();
		FileView(const char* root) : rootPath(root) {};
		~FileView();

		void OnWindowRender() override;

	private:
		const char* rootPath = "";
	};
}