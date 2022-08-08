#pragma once

#include "Hazard.h"
#include "Core/EditorEvent.h"

namespace UI {
	class Properties : public Hazard::ImUI::Panel {
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
			ImUI::MenuItem(title, [&]() {
				e.AddComponent<T>();
				});
		}
	}
}