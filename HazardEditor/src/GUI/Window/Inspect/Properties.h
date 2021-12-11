#pragma once

#include "Core/EditorEvent.h"
#include "Hazard.h"
#include "GUI/EditorWindow.h"

namespace WindowElement {

	class Properties : public EditorWindow {
	public:
		Properties();
		~Properties() = default;

		void OnUpdate() override;
		void OnWindowRender() override;
		bool OnEvent(Event& e) override;
		bool SelectionContextChange(Events::SelectionContextChange& e);

		bool IsLocked() { return locked; }
		void SetLocked(bool locked) { this->locked = locked; }

	private:
		Hazard::ECS::Entity selectionContext;
		bool locked = false;
	};
}