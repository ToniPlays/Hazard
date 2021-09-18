#pragma once
#include "Hazard.h"
#include "Core/EditorEvent.h"
#include "GUI/EditorWindow.h"

using namespace Hazard;
using namespace Hazard::ECS;

namespace WindowElement {
	class Hierarchy : public EditorWindow {
	public:
		Hierarchy();
		~Hierarchy() = default;

		bool OnEvent(Event& e) override;

		void OnWindowRender() override;
		bool SelectionContextChange(Events::SelectionContextChange& e);

	private:
		void DrawEntity(Entity entity);
		bool KeyPressEvent(KeyPressedEvent& e);

	private:
		WorldHandler* handler = nullptr;
		Entity selectionContext;

		std::string searchValue;
	};
}