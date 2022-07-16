#pragma once

#include "GUI/Core/Panel.h"

namespace UI {
	class Properties : public Panel {
	public:
		Properties();

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;
		bool OnSelectionContextChange(Events::SelectionContextChange& e);
	private:
		void DrawContextMenu(Entity& e);
		template<typename T>
		void DrawAddComponentMenuIfNotExists(const char* title, Entity& e);
	private:
		Entity m_SelectionContext;
	};
	template<typename T>
	inline void Properties::DrawAddComponentMenuIfNotExists(const char* title, Entity& e)
	{
		if (!e.HasComponent<T>()) {
			UI::MenuItem(title, [&]() {
				e.AddComponent<T>();
				});
		}
	}
}