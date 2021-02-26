#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

using namespace Hazard;
using namespace Hazard::ECS;

namespace WindowElement {
	class Hierarchy : public EditorWindow {
	public:
		Hierarchy();
		~Hierarchy();

		void Init() override;

		void OnWindowRender() override;

		void SetSelectionContext(Entity entity) { selectionContext = entity; }

	private:
		SceneHandler* handler;
		Entity selectionContext;
	private:
		void DrawEntity(Entity entity);
	};
}