#pragma once

#include "Hazard.h"
#include "Hazard/Core/Application.h"
#include "Hazard/ECS/WorldHandler.h"
#include "Hazard/ECS/Entity.h"
#include "Core/EditorEvent.h"
#include "Hazard/ImGUI/UIElements/TextField.h"
#include "Hazard/ImGUI/Panel.h"

namespace UI 
{
	class Hierarchy : public Hazard::ImUI::Panel {
	public:
		Hierarchy();

		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;
		bool OnKeyPressed(KeyPressedEvent& e);
        bool OnEntitiesSelected(Events::SelectionContextChange& e);

	private:
		void DrawModifiers(Hazard::Entity& e, Hazard::TagComponent& tag);
		void DrawContextMenu(Ref<Hazard::World>& world);
		void SelectEntity(const Hazard::Entity& entity);
		void DeselectEntity(const Hazard::Entity& entity);
		void ClearSelected();

	private:
		std::vector<Hazard::Entity> m_SelectionContext;
		Hazard::ImUI::TextField m_SearchField;
	};
}
