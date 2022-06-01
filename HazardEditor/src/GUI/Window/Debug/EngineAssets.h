#pragma once
#include <hzrpch.h>
#include "GUI/EditorWindow.h"


namespace WindowElement {
	class EngineAssets : public EditorWindow {
	public:
		EngineAssets();
		~EngineAssets() = default;

		void OnWindowRender() override;
	};
}