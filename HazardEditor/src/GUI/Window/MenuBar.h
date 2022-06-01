#pragma once

#include "GUI/RenderableElement.h"
#include "Backend/Core/Events.h"

using namespace HazardRenderer;

namespace WindowElement {

	class MenuBar : public RenderableElement {
	public:
		MenuBar() = default;
		~MenuBar() = default;

		void OnMenuRender() override;
		bool OnEvent(Event& e) override;
	private:
		void DrawMenu();

		bool KeyPressed(KeyPressedEvent& e);
		glm::vec2 dragPos = { 0, 0 };
		bool m_TitleBarHovered;
	};
}