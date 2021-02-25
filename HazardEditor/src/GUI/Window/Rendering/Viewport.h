#pragma once

#include "GUI/EditorWindow.h"

namespace WindowElement {
	class Viewport : public EditorWindow {
	public:
		Viewport();
		~Viewport();

		void OnWindowRender() override;

	private:

	};
}