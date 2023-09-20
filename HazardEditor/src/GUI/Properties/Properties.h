#pragma once

#include "Hazard.h"
#include "Core/EditorEvent.h"
#include "Editor/EditorUtils.h"

namespace UI
{
	class Properties : public Hazard::ImUI::Panel
	{
	public:
		Properties();

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;
		bool OnSelectionContextChange(Events::SelectionContextChange& e);
	private:
		void DrawContextMenu(std::vector<Entity>& e);
		template<typename T>
		void DrawAddComponentMenuIfNotExists(const char* title, std::vector<Entity>& entities);

	private:
		std::vector<Entity> m_SelectionContext;
	};
	template<typename T>
	inline void Properties::DrawAddComponentMenuIfNotExists(const char* title, std::vector<Entity>& entities)
	{
		bool canAdd = !AllEntitiesContain<T>(entities);

		if (!canAdd) ImGui::BeginDisabled();
		ImUI::MenuItem(title, [&]() {
			AddComponentToAll<T>(m_SelectionContext);
		});
		if (!canAdd) ImGui::EndDisabled();
	}
}