#pragma once

#include "Hazard.h"
#include "Hazard/Core/Application.h"
#include "Hazard/ECS/WorldHandler.h"
#include "Hazard/ECS/Entity.h"

namespace UI {
	class Hierarchy : public Hazard::ImUI::Panel {
	public:
		Hierarchy();

		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;
		bool OnKeyPressed(KeyPressedEvent& e);
	private:
		void DrawModifiers(Hazard::Entity& e, Hazard::TagComponent& tag);
		void DrawContextMenu(Ref<Hazard::World>& world);
	private:
		Hazard::WorldHandler* m_WorldHandler;
		Hazard::Entity m_SelectionContext;

		std::string m_SearchValue;
	};
}