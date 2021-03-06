#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

namespace WindowElement {

	class Properties : public EditorWindow {
	public:
		Properties();
		~Properties();

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