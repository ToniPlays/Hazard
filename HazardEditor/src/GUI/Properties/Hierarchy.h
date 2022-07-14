#pragma once

#include "GUI/Core/Panel.h"
#include "Hazard/Core/Application.h"
#include "Hazard/ECS/WorldHandler.h"
#include "Hazard/ECS/Entity.h"

namespace UI {
	class Hierarchy : public Panel {
	public:
		Hierarchy();

		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };
	private:
		//ECS world here
		Hazard::WorldHandler* m_WorldHandler;
	};
}