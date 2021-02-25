#pragma once

#include "GUI/EditorWindow.h"

namespace WindowElement {
	class Hierarchy : public EditorWindow {
	public:
		Hierarchy();
		~Hierarchy();

		void OnWindowRender() override;

	private:

	};
}