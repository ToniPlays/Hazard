#pragma once

#include "GUI/EditorWindow.h"

namespace WindowElement {
	class FileView : public EditorWindow {
	public:
		FileView();
		~FileView();

		void OnWindowRender() override;

	private:

	};
}