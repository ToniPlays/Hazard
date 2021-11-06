#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

#include "Editor/EditorCamera.h"

namespace WindowElement {

	class WorldEnvironmentData : public EditorWindow {
	public:
		WorldEnvironmentData();
		~WorldEnvironmentData() = default;

		void OnWindowRender() override;
	};
}