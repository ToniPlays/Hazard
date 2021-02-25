#pragma once

#include "GUI/EditorWindow.h"

namespace WindowElement {
	class Performance : public EditorWindow {
	public:
		Performance();
		~Performance();

		void OnWindowRender() override;

	private:

	};
}