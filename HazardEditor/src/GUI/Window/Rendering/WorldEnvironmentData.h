#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"
#include "GUI/Window/TransformationGizmo.h"

#include "Editor/EditorCamera.h"

namespace WindowElement {

	class WorldEnvironmentData : public EditorWindow {
	public:
		WorldEnvironmentData();
		~WorldEnvironmentData();

		void OnWindowRender() override;
	};
}