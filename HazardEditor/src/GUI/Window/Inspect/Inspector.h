#pragma once

#include "GUI/EditorWindow.h"

namespace WindowElement {

	class Inspector : public EditorWindow {
	public:
		Inspector();
		~Inspector();

		void OnWindowRender() override;

	private:

	};
}