#pragma once

#include "GUI/EditorTabWindow.h"

namespace WindowElement {
	class EditorMainTab : public EditorTabWindow {
	public:

		EditorMainTab() : EditorTabWindow("Editor") {};

		virtual void Init() override;
		virtual void Render() override;
		bool OnEvent(Event& e) override;
	};
}