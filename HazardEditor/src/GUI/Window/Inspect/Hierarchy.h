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
		~Hierarchy();

		void Init() override;
		bool OnEvent(Event& e) override;

		void OnWindowRender() override;
		bool SelectionContextChange(Events::SelectionContextChange& e);

	private:
		SceneHandler* handler;
		Entity selectionContext;

	private:
		void DrawEntity(Entity entity);
	};
}