#pragma once

#include "GUI/RenderableElement.h"

namespace WindowElement {
	class MenuBar : public RenderableElement {
	public:
		MenuBar();
		~MenuBar();

		void OnMenuRender() override;
		bool OnEvent(Event& e) override;
	private:
		bool KeyPressed(KeyPressedEvent& e);
	};
}