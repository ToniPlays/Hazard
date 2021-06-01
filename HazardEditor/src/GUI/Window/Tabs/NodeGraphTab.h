#pragma once

#include "GUI/EditorTabWindow.h"

namespace WindowElement {
	class NodeGraphTab : public EditorTabWindow {
	public:

		NodeGraphTab() : EditorTabWindow("Node graph") {};

		virtual void Init() override;
		virtual void Render() override;
		bool OnEvent(Event& e) override;
	};
}