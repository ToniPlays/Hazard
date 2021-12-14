#pragma once

#include "Core/EditorEvent.h"
#include "Hazard.h"
#include "GUI/EditorWindow.h"

namespace WindowElement {

	class Properties : public EditorWindow {
	public:
		Properties();
		~Properties() = default;

		void Init();
		void OnUpdate() override;
		void OnWindowRender() override;
		bool OnEvent(Event& e) override;
		bool SelectionContextChange(Events::SelectionContextChange& e);

		bool IsLocked() { return m_Locked; }
		void SetLocked(bool locked) { this->m_Locked = locked; }

	private:
		Hazard::ECS::Entity m_SelectionContext;
		bool m_Locked = false;
	};
}